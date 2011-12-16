__author__ = 'andrew'
__IMAGE_PATH__ = '/home/andrew/OS/plan9d'
import  sys, os,re ,popen2


def get_candidates():
    file =open(__IMAGE_PATH__,'r')
    result = []
    size = os.path.getsize(__IMAGE_PATH__)
    position = 0
    try:
        while position +512 <= size:
            file.seek(position+510,1)
            readed = file.read(2)
            #print repr(readed)
           # break
            if  readed == '\x55\xaa':
                result.append(position)
            position+=512
    finally:
        return result

file =open(__IMAGE_PATH__,'r')
for pos in  get_candidates():
    print "\n;--Candidate at %d--" % (pos,)
    file.seek(pos,1)
    sector = file.read(512)
    tmp_file =open("/tmp/os.bin",'w')
    tmp_file.write(sector)
    tmp_file.close()
    ndisasm_out, ndisasm_in = popen2.popen2("ndisasm /tmp/os.bin")
    sys.stdout.write(ndisasm_out.read())

