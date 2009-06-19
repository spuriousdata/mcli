PYGEN=pyuic4

PY=ui_mci.py ui_connect_dialog.py

uipy: $(PY)

config:
	cp mci.ini-sample mci.ini

all: uipy config

%.py : %.ui
	$(PYGEN) $< > $@

clean:
	rm -f $(PY) *.pyc
