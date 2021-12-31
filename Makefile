
SUBDIRS = 20*

.PHONY: subdirs $(SUBDIRS) clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm */*/*.o */*/*.out