PYGEN=pyuic4

PY=ui_mci.py ui_connect_dialog.py

uipy: $(PY)

all: uipy

%.py : %.ui
	$(PYGEN) -d $< > $@

