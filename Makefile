default: mimg.so

CFLAGS += -I$(HOME)/.runt/include -fPIC
LDFLAGS += -L$(HOME)/.runt/lib 
LDFLAGS += -lmonomer
LDFLAGS += -lrunt_patchwerk
LDFLAGS += -lrunt
LDFLAGS += -lsoundpipe
LDFLAGS += -lrunt_img

mimg.so: mimg.c
	$(CC) $(CFLAGS) -shared $< -o $@ $(LDFLAGS) 

install: mimg.so
	mkdir -p $(HOME)/.runt/plugins/monomer
	cp mimg.so $(HOME)/.runt/plugins/monomer

clean:
	$(RM) mimg.so
