
XA= XAINPUT=../oadef,.. xa

all: gecko inigecko

g gecko: gecko.a65 oadev.a65 fsiec.a65 keydev.a65 irtx.a65
	${XA} gecko.a65 -o gecko

inigecko: inigecko.a65
	${XA} inigecko.a65 -o inigecko

inig2: inig2.a65
	${XA} inig2.a65 -o inig2

fsiec: fsiec.a65
	${XA} fsiec.a65 -o fsiec

clean:
	rm -f gecko inigecko fsiec
