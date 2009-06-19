PYGEN=pyuic4

PY=ui_mci.py ui_connect_dialog.py

all: uipy config

uipy: $(PY)

config: mci.ini

mci.ini : mci.ini-sample
	cp mci.ini-sample mci.ini

%.py : %.ui
	$(PYGEN) $< > $@

clean:
	rm -f $(PY) *.pyc
