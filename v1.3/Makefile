

#XAPATH=~/src/xa-2.1.0/xa
XAPATH=xa 
XAINPUT="..,../oadef,../oa1"
XAOUTPUT=".."

XA=XAINPUT="${XAINPUT}" XAOUTPUT="${XAOUTPUT}" ${XAPATH} 

all: gecko.rom rom ibmon mcopy c64 slip rmerr 

.obj.a65:
	(cd $%; ${XA} -x $%);
	
gecko.rom: 
	(cd gecko; make );
	mv gecko/gecko gecko.rom

rom: 
	(cd stdrom; ${XA} -x rom.a65);

inirom: 
	(cd stdrom; ${XA} -x inirom.a65);

c64: 
	(cd c64rom; ${XA} -x c64rom.a65);

basic:
	(cd oabasic; ${XA} -x oabasic4.a65);

ibmon:
	(cd ibmon; ${XA} -x ibmon.a65);

mcopy:
	(cd mcopy; ${XA} -x mcopy.a65);

fsiec:
	(cd oafs; ${XA} -x fsiec.a65);

fsibm:
	(cd oafs; ${XA} -x fsibm.a65);

fsdev:
	(cd oafs; ${XA} -x fsdev.a65);

fstcp: slip
	(cd oaslip; ${MAKE} fstcp);

sh:
	(cd oash; ${XA} -x sh.a65);

slip: 
	(cd oaslip; ${XA} main.a65 -o slip.obj -l slip.lab);

rmerr:
	rm -f *.lab *.err

clean: rmerr
	rm -f *.obj *.rom
	(cd oaslip; ${MAKE} clean);
	(cd gecko; ${MAKE} clean);


