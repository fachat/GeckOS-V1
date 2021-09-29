
FILES=loader boot

all: ${FILES}

loader: loader.lst
	petcat -w40 $< > $@

boot: boot.a65
	xa -o $@ $<


clean:
	rm -f ${FILES}
