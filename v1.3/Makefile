

XAINPUT="..,../oadef,../oa1"
XAOUTPUT=".."

XA=XAINPUT="${XAINPUT}" XAOUTPUT="${XAOUTPUT}" xa

all: rom basic ibmon mcopy fsiec fsibm fsdev sh rmerr

rom: 
	(cd stdrom; ${XA} rom.a65);

basic:
	(cd oabasic; ${XA} oabasic4.a65);

ibmon:
	(cd ibmon; ${XA} ibmon.a65);

mcopy:
	(cd mcopy; ${XA} mcopy.a65);

fsiec:
	(cd oafs; ${XA} fsiec.a65);

fsibm:
	(cd oafs; ${XA} fsibm.a65);

fsdev:
	(cd oafs; ${XA} fsdev.a65);

sh:
	(cd oash; ${XA} sh.a65);

rmerr:
	rm -f *.LAB *.ERR

clean: rmerr
	rm -f *.OBJ

