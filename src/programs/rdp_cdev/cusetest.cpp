#define FUSE_USE_VERSION 30
#define _FILE_OFFSET_BITS 64
#include <fuse3/cuse_lowlevel.h>
#include <fuse3/fuse_opt.h>
#include <string.h>
// #include <stdio.h>
#include <iostream>

#include <rdp_cuse.h>
rdp::RdpValueStore * the_store = NULL;

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
    auto single_value = the_store->get_value();

    size_t buffer_size = sizeof(time_t)+sizeof(rdp::RdpValueType)+sizeof(float);
    char buffer[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    rdp::RdpValue2Buffer(single_value,buffer);
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
    fuse_reply_ioctl(req, 0, NULL, 0);
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
    std::string values="/etc/rdp_values.csv";
    time_t now=0;
    const std::string  value_key="--values";
    const std::string time_key="--time";
    std::string last_key="";
    for(int i=1;i < argc; i++) {
        std::clog << "Managing arg: " << argv[i] << std::endl;
        if(last_key=="") {
            last_key=argv[i];
        } else if(last_key==value_key) {
            values=argv[i];
            last_key="";
        } else if(last_key==time_key) {
            now=atoi(argv[i]);
            last_key="";
        } else {
            std::clog << "Unknown key " << last_key << std::endl;
        }
    }
    std::clog << "Starting rdp cuse with values from " << values << " and an unix time of: " << now << std::endl;
    rdp::RdpValueStore local_store = rdp::RdpValueFactory::create(now,values.c_str());
    std::clog << "Managed to read " << local_store.get_value_count() << " values from " << values << std::endl;
    the_store = &local_store;

    const char* cusearg[] = {"test", "-f", "-d"};
    const char* devarg[]  = {"DEVNAME=rdp_cdev" };
    struct cuse_info ci;
    memset(&ci, 0x00, sizeof(ci));
    ci.flags = CUSE_UNRESTRICTED_IOCTL;
    ci.dev_major = 234;
    ci.dev_minor = 0;
    ci.dev_info_argc=1;
    ci.dev_info_argv = devarg;

    return cuse_lowlevel_main(3, (char**) &cusearg, &ci, &cusetest_clop, NULL);
}
