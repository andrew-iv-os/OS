__author__ = 'andrew'
__IMAGE_PATH__ = '/home/andrew/OS/plan9d'
import  sys, os,re 

""" CRC16-CCITT hash, part of Battlefield 2142 Auth token maker
This is the python module package for computing CRC16-CCITT hash.
"""

table = (
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b,
        0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
        0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401,
        0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738,
        0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96,
        0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
        0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
        0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
        0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb,
        0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2,
        0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
        0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
        0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827,
        0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
        0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d,
        0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
        0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
        0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
        )

def crc16_compute(data):
    """ Compute correct enough :grin: CRC16 CCITT for using in BF2142 auth token """
    crc = 0
    for byte in (part for part in data):
        ushort = (crc << 8) & 0xff00
        crc = ((ushort) ^ table[((crc >> 8) ^ (0xff & byte))])
    return crc

template = r"""
org 0x7c00
cli
xor ax, ax
cli
mov ds, ax
mov es, ax
mov ss, ax
mov sp,0x7b00
sti
mov al, [SectorsToLoad]
mov ah, 02h
mov bx, CRC16_DATA
mov cx, 0002h
int 0x13
call crc16
cmp ax,[OrigCRC]
jnz _end
mov ax, 0x1301
mov bx, 0x0007
mov cx, end2-MYFILE
mov bp, MYFILE
int 0x10
_end:
cli
hlt


crc16:
	push cx
	push bx
	push dx
	mov dx, 0ffffh
	mov si,MYFILE
	std
	mov cx,%(len)d
	.lp_crc:
	lodsb
	mov bx,dx
	shr bx,8
	shl bx,1
	xor ax,[bx+CRC16_DATA]
	shl dx,8
	xor dx,ax
	loop .lp_crc
	mov ax,dx
	pop dx
	pop bx
	pop cx
ret
SectorsToLoad:
db %(sectors)d
OrigCRC: dw %(crc)d
times 510-($-$$) db 0
db 0x55, 0xaa
CRC16_DATA:	dw	00000h, 01021h, 02042h, 03063h, 04084h, 050A5h, 060C6h, 070E7h
		dw	08108h, 09129h, 0A14Ah, 0B16Bh, 0C18Ch, 0D1ADh, 0E1CEh, 0F1EFh
		dw	01231h, 00210h, 03273h, 02252h, 052B5h, 04294h, 072F7h, 062D6h
		dw	09339h, 08318h, 0B37Bh, 0A35Ah, 0D3BDh, 0C39Ch, 0F3FFh, 0E3DEh
		dw	02462h, 03443h, 00420h, 01401h, 064E6h, 074C7h, 044A4h, 05485h
		dw	0A56Ah, 0B54Bh, 08528h, 09509h, 0E5EEh, 0F5CFh, 0C5ACh, 0D58Dh
		dw	03653h, 02672h, 01611h, 00630h, 076D7h, 066F6h, 05695h, 046B4h
		dw	0B75Bh, 0A77Ah, 09719h, 08738h, 0F7DFh, 0E7FEh, 0D79Dh, 0C7BCh
		dw	048C4h, 058E5h, 06886h, 078A7h, 00840h, 01861h, 02802h, 03823h
		dw	0C9CCh, 0D9EDh, 0E98Eh, 0F9AFh, 08948h, 09969h, 0A90Ah, 0B92Bh
		dw	05AF5h, 04AD4h, 07AB7h, 06A96h, 01A71h, 00A50h, 03A33h, 02A12h
		dw	0DBFDh, 0CBDCh, 0FBBFh, 0EB9Eh, 09B79h, 08B58h, 0BB3Bh, 0AB1Ah
		dw	06CA6h, 07C87h, 04CE4h, 05CC5h, 02C22h, 03C03h, 00C60h, 01C41h
		dw	0EDAEh, 0FD8Fh, 0CDECh, 0DDCDh, 0AD2Ah, 0BD0Bh, 08D68h, 09D49h
		dw	07E97h, 06EB6h, 05ED5h, 04EF4h, 03E13h, 02E32h, 01E51h, 00E70h
		dw	0FF9Fh, 0EFBEh, 0DFDDh, 0CFFCh, 0BF1Bh, 0AF3Ah, 09F59h, 08F78h
		dw	09188h, 081A9h, 0B1CAh, 0A1EBh, 0D10Ch, 0C12Dh, 0F14Eh, 0E16Fh
		dw	01080h, 000A1h, 030C2h, 020E3h, 05004h, 04025h, 07046h, 06067h
		dw	083B9h, 09398h, 0A3FBh, 0B3DAh, 0C33Dh, 0D31Ch, 0E37Fh, 0F35Eh
		dw	002B1h, 01290h, 022F3h, 032D2h, 04235h, 05214h, 06277h, 07256h
		dw	0B5EAh, 0A5CBh, 095A8h, 08589h, 0F56Eh, 0E54Fh, 0D52Ch, 0C50Dh
		dw	034E2h, 024C3h, 014A0h, 00481h, 07466h, 06447h, 05424h, 04405h
		dw	0A7DBh, 0B7FAh, 08799h, 097B8h, 0E75Fh, 0F77Eh, 0C71Dh, 0D73Ch
		dw	026D3h, 036F2h, 00691h, 016B0h, 06657h, 07676h, 04615h, 05634h
		dw	0D94Ch, 0C96Dh, 0F90Eh, 0E92Fh, 099C8h, 089E9h, 0B98Ah, 0A9ABh
		dw	05844h, 04865h, 07806h, 06827h, 018C0h, 008E1h, 03882h, 028A3h
		dw	0CB7Dh, 0DB5Ch, 0EB3Fh, 0FB1Eh, 08BF9h, 09BD8h, 0ABBBh, 0BB9Ah
		dw	04A75h, 05A54h, 06A37h, 07A16h, 00AF1h, 01AD0h, 02AB3h, 03A92h
		dw	0FD2Eh, 0ED0Fh, 0DD6Ch, 0CD4Dh, 0BDAAh, 0AD8Bh, 09DE8h, 08DC9h
		dw	07C26h, 06C07h, 05C64h, 04C45h, 03CA2h, 02C83h, 01CE0h, 00CC1h
		dw	0EF1Fh, 0FF3Eh, 0CF5Dh, 0DF7Ch, 0AF9Bh, 0BFBAh, 08FD9h, 09FF8h
		dw	06E17h, 07E36h, 04E55h, 05E74h, 02E93h, 03EB2h, 00ED1h, 01EF0h
MYFILE: db %(data)s
end2:

"""


def asm_data(bytes):
    import  string
    return string.join(map(lambda  XX : " 0x%X " % (XX,) , bytes),',')


if __name__ == '__main__':
    from array import  array
    content =  array("B",open(sys.argv[1],'r').read())
    print content
    data = str(asm_data(content))
    if(len(data) > 30*1024):
        print "The file is too large"
        sys.exit()

    asm_code = (template % {"len":len(content),"sectors":((len(content)-1)/512+2),"data":data,"crc":crc16_compute(content)})
    asm_file = open('/tmp/os.asm','w')
    asm_file.write(asm_code)
    asm_file.close()
    os.system('nasm /tmp/os.asm -o task3.bin')
    bin_file =  open('task3.bin','a')
    bin_file.write(' ' * 512)
    bin_file.close()


