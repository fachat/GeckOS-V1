

#XAPATH=~/src/xa-2.1.0/xa
XAPATH=xa 
XAINPUT="..,../oadef,../oa1"
XAOUTPUT=".."

XA=XAINPUT="${XAINPUT}" XAOUTPUT="${XAOUTPUT}" ${XAPATH} 

all: rom mcopy c64 slip rmerr prgs csaboot.obj

.obj.a65:
	(cd $%; ${XA} -o %@ $%);
	
gecko.rom: 
	(cd gecko; make );
	mv gecko/gecko gecko.rom

rom: 
	(cd stdrom; ${XA} -o rom.obj rom.a65);

inirom: 
	(cd stdrom; ${XA} -o initrom.obj inirom.a65);

c64: 
	(cd c64rom; ${XA} -o c64rom.obj c64rom.a65);

basic:
	(cd oabasic; ${XA} -o oabasic4.obj oabasic4.a65);

prgs:
	make -C ibmon

csaboot.obj:
	make -C csaboot
mcopy:
	(cd mcopy; ${XA} -o mcopy mcopy.a65);

fsiec:
	(cd oafs; ${XA} -o fsiec.obj fsiec.a65);

fsibm:
	(cd oafs; ${XA} -o fsibm.obj fsibm.a65);

fsdev:
	(cd oafs; ${XA} -o fsdev.obj fsdev.a65);

fstcp: slip
	(cd oaslip; ${MAKE} fstcp);

sh:
	(cd oash; ${XA} -o sh.obj sh.a65);

slip: 
	(cd oaslip; ${XA} main.a65 -o slip.obj -l slip.lab);

rmerr:
	rm -f *.lab *.err

clean: rmerr
	rm -f *.obj *.rom
	(cd oaslip; ${MAKE} clean);
	(cd gecko; ${MAKE} clean);
	(cd csaboot; ${MAKE} clean);
	(cd ibmon; ${MAKE} clean);

.phony: ibmon

