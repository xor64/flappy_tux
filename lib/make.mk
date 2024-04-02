

#? Sample library build recipe:
#? Prefferably put this in the lib directory

#? The onlu thing needed for root makefile, you can build it another way if u want
# LIBS += bin/lib/libmc.a
# 
# libmc_src = $(wildcard lib/libmc/src/*.c)
# libmc_obj = $(patsubst lib/libmc/src/%.c,bin/lib/libmc/%.c.o,$(libmc_src))
# 
# bin/lib/libmc.a: $(libmc_obj)
#	 @mkdir -p $(dir $@)
#	 ar rcs $@ $^
# 
# bin/lib/libmc/%.c.o: lib/libmc/src/%.c
#  	@mkdir -p $(dir $@)
#  	$(CC) -c -o $@ $<





