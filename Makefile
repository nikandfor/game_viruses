CXXFLAGS	= -g -Wall -c -std=c99

BINDIR		= ./bin
OBJDIR		= $(BINDIR)/.objs
TEST_BIN	= $(BINDIR)/test
RUN_BIN		= $(BINDIR)/run
UTILSDIR	= utils
SRCDIR		= src
GTK_FLAGS	= `pkg-config --cflags gtk+-3.0`
GTK_LIBS	= `pkg-config --libs gtk+-3.0`

INCLUDE		= -Iinclude

$(RUN_BIN): $(OBJDIR)/main.o
	@[ -d $(BINDIR) ] || mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(GOO_LIBS) $(GTK_LIBS)

$(TEST_BIN): $(OBJDIR)/misc.o
	@[ -d $(BINDIR) ] || mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(GOO_LIBS) $(GTK_LIBS)

$(OBJDIR)/%.o:: $(SRCDIR)/%.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $@ $< $(GTK_FLAGS)

$(OBJDIR)/%.o:: $(UTILSDIR)/%.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $@ $< $(GTK_FLAGS)

#$(OBJDIR)/misc.o: include/data.h

clean:
	rm -rf $(TEST_BIN) $(RUN_BIN) $(OBJDIR)/*

.PHONY: clean
