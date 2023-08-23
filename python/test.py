import struct

print("Opening it")
with open("/dev/rdp_cdev","rb") as f:
    print("Yes it's open")
    test = f.read(16)
    for i in range(16):
        print("%.2X"%test[i],end=" ")
        if (i%2):
            print("  ", end="")
    time = 0
    for i in range(8):
        time |= test[i] << 8*i
    type_num = 0
    for i in range(4):
        type_num |= test[i+8] << 8*i
    value = 0.0
    print()
    print (test[-4::])
    value=struct.unpack('f',test[-4::])
    # for i in range(4):
    #     value |= test[i+12] << 8*i
    print()
    print(value)
    print("Read one time: %d type :%d and value %f" % (time,type_num,value[0]))