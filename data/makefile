



.PHONY: debug
debug: 
	@javac bootstrap.java
	@xbuild embedsanity/embedsanity.sln /verbosity:quiet
	@$(RM) ./mono_interop.exe
	@ln -s `pwd`/embedsanity/bin/Debug/embedsanity.exe ./mono_interop.exe
