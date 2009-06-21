PYGEN=pyuic4

PYC=$(shell echo `ls *ui *py` | sed 's/\.py/\.pyc/g' | sed 's/\.ui/\.pyc/g')

all: config objs

objs: $(PYC)

config: mci.ini

mci.ini : mci.ini-sample
	cp mci.ini-sample mci.ini

%.py : %.ui
	$(PYGEN) $< > $@

%.pyc : %.py
	python -m py_compile $<

clean:
	rm -f ui_*.py *.pyc
