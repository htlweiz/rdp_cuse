#define FUSE_USE_VERSION 30
#define _FILE_OFFSET_BITS 64
#include <fuse3/cuse_lowlevel.h>
#include <fuse3/fuse_opt.h>
#include <string.h>
// #include <stdio.h>
#include <iostream>

#include <rdp_cuse.h>

using std::cout, std::cerr, std::endl;

#define LOG cerr << "LOG: "

// #define LOG(...) do { fprintf(stderr, __VA_ARGS__); puts(""); } while (0)

static void cusetest_open(fuse_req_t req, struct fuse_file_info *fi) {
    LOG << "open with req: " << fi << endl;
    fuse_reply_open(req, fi);
}

static void cusetest_read(fuse_req_t req, size_t size, off_t off,
                         struct fuse_file_info *fi) {
    LOG << "read with req:" << req << endl;
    rdp::RdpValue value_1(rdp::RDP_TEMP_VALUE_TYPE,1000,22.2);

    time_t time = value_1.get_time();
    unsigned int type = value_1.get_type();
    float value = value_1.get_value();

    size_t buffer_size = sizeof(time)+sizeof(type)+sizeof(value);

    void * buffer;
    buffer = malloc(buffer_size);
    size_t position=0;
    memset(buffer,0,buffer_size);
    memcpy(buffer+position,&time,sizeof(time));
    position+=sizeof(time);
    memcpy(buffer+position,&type,sizeof(type));
    position+=sizeof(type);
    memcpy(buffer+position,&value,sizeof(value));
    position+=sizeof(value);
    std::cout << "Position is: " << position << std::endl;
    std::cout << "Sizeof time: " << sizeof(time) << std::endl;
    std::cout << "Sizeof type: " << sizeof(type) << std::endl;
    std::cout << "Sizeof value: " << sizeof(value) << std::endl;
    const char * buffer_pointer=0;
    buffer_pointer = (const char *) &buffer;
    fuse_reply_buf(req, buffer_pointer, size > buffer_size ? buffer_size : size);
}

static void cusetest_write(fuse_req_t req, const char *buf, size_t size,
                          off_t off, struct fuse_file_info *fi) {
    LOG << "write " << size << " bytes to req: " << req << endl;
    fuse_reply_write(req, size);
}

static void cusetest_ioctl(fuse_req_t req, int cmd, void *arg,
                          struct fuse_file_info *fi, unsigned flags,
                          const void *in_buf, size_t in_bufsz, size_t out_bufsz) {
    LOG << "ioctl "<< cmd << ": insize: " << in_bufsz << " outsize: " << out_bufsz << endl;
    switch (cmd) {
    case 23:
        if (in_bufsz == 0) {
            struct iovec iov = { arg, sizeof(int) };
            fuse_reply_ioctl_retry(req, &iov, 1, NULL, 0);
        } else {
            LOG << "  got value: " << *((int*)in_buf) << endl;
            fuse_reply_ioctl(req, 0, NULL, 0);
        }
        break;
    case 42:
        if (out_bufsz == 0) {
            struct iovec iov = { arg, sizeof(int) };
            fuse_reply_ioctl_retry(req, NULL, 0, &iov, 1);
        } else {
            LOG << "  write back value" << endl;
            int v = 42;
            fuse_reply_ioctl(req, 0, &v, sizeof(int));
        }
        break;
    }
}

static const struct cuse_lowlevel_ops cusetest_clop = {
        .open           = cusetest_open,
        .read           = cusetest_read,
        .write          = cusetest_write,
        .ioctl          = cusetest_ioctl,
};

int main(int argc, char** argv) {
    // -f: run in foreground, -d: debug ouput
    // Compile official example and use -h
    const char* cusearg[] = {"test", "-f", "-d"};
    const char* devarg[]  = {"DEVNAME=rdp_cdev" };
    struct cuse_info ci;
    memset(&ci, 0x00, sizeof(ci));
    ci.flags = CUSE_UNRESTRICTED_IOCTL;
    ci.dev_info_argc=1;
    ci.dev_info_argv = devarg;

    return cuse_lowlevel_main(3, (char**) &cusearg, &ci, &cusetest_clop, NULL);
}