bin_PROGRAMS = anq

LDADD = $(GPGME_LIBS)

anq_CPPFLAGS = -Isrc/ \
	       -Isrc/utils/ \
	       -Isrc/crypto/ \
	       -Isrc/configuration/ \
	       $(GPGME_CFLAGS)

include_HEADERS = src/err_codes.h \
		  src/utils/io_utils.h \
		  src/crypto/crypto_ops.h \
		  src/crypto/crypto_data.h \
		  src/crypto/gpgme/crypto.h \
		  src/configuration/parser.h \
		  src/configuration/argv_parser.h \
		  src/configuration/config_parser.h

anq_SOURCES  = src/main.c \
	       src/err_codes.c \
	       src/utils/io_utils.c \
	       src/crypto/crypto_ops.c \
	       src/crypto/crypto_data.c \
	       src/crypto/gpgme/crypto.c \
	       src/configuration/parser.c \
	       src/configuration/argv_parser.c \
	       src/configuration/config_parser.c
