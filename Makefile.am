bin_PROGRAMS = anq

LDADD = $(GPGME_LIBS)

anq_CPPFLAGS = -Isrc/ \
	       -Isrc/utils/ \
	       -Isrc/crypto/ \
	       -Isrc/configuration/ \
	       $(GPGME_CFLAGS)

anq_SOURCES  = src/main.c \
	       src/configuration/parser.c \
	       src/err_codes.c \
	       src/utils/io_utils.c \
	       src/crypto/crypto_ops.c \
	       src/crypto/crypto_data.c \
	       src/crypto/gpgme/crypto.c \
	       src/configuration/argv_parser.c \
	       src/configuration/config_parser.c
