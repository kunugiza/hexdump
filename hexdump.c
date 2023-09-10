/*
 * Hexdump implemented with no library dependencies including libc.
 *
 * Written by @kunukunu (Junsuke Kunugiza)
 * Public domain.
 */


/* Compile time configurations */

#define OCTETS_PER_LINE		16
#define SP_BEFORE_ADDRESS	2
#define SP_AFTER_ADDRESS	2
#define SP_AFTER_HEXDUMP	2
#define SP_AFTER_OCTET		1
#define SP_DIVIDER		1
#define OCTET_WITH_DIVIDER	8
#define LEN_ADDRESS		4

#define NEWLINE1		'\n'
#define NEWLINE2		'\0'


/* ------------------------------------------------------------------------ */

/* 
 * Example only.
 * Implement whatever you need to print out the formatted string.
 */

#include <stdio.h>
#include <stdlib.h>

static void
send_line(const char *str, void *arg) 
{
  (void)arg;
  fputs(str, stdout);
  fflush(stdout);
}

/* ------------------------------------------------------------------------ */

#define LEN_HEX_OCTET	(2 + SP_AFTER_OCTET)
#define LEN_HEXDUMP	(LEN_HEX_OCTET * OCTETS_PER_LINE)
#define LEN_ASCII_DUMP	OCTETS_PER_LINE

#define POS_ADDRESS	SP_BEFORE_ADDRESS
#define POS_HEXDUMP	(POS_ADDRESS + LEN_ADDRESS + 2 + SP_AFTER_ADDRESS)
#define POS_ASCII	(POS_HEXDUMP + LEN_HEXDUMP + SP_AFTER_HEXDUMP)
#define POS_NEWLINE1	(POS_ASCII + OCTETS_PER_LINE + 2)
#define POS_NEWLINE2	(POS_NEWLINE1 + 1)
#define POS_NULLTERM	(POS_NEWLINE2 + 1)

#define LINE_LENGTH	(POS_NULLTERM + 1)

#define GET_POS_HEX_OCTET_L(pos) (POS_HEXDUMP + (LEN_HEX_OCTET * pos))
#define GET_POS_HEX_OCTET_H(pos) (POS_HEXDUMP + SP_DIVIDER + (LEN_HEX_OCTET * pos))
#define GET_POS_ASCII(pos)       (POS_ASCII + 1 + pos)

#define IS_PRINTABLE(c) (c >= 0x20 && c <= 0x7e)


void
hexdump_std(const void *data, size_t len, size_t addr, void *arg)
{
  size_t count, addr_val;
  int pos, i;
  const unsigned char *p;
  char line[LINE_LENGTH];

  const char hex[] = "0123456789ABCDEF";

  for (count = 0; count < sizeof(line); count++) {
    line[count] = ' ';
  }

  line[POS_ADDRESS + LEN_ADDRESS + 1]   = ':';
  line[POS_ASCII]                       = '|';
  line[POS_ASCII + OCTETS_PER_LINE + 1] = '|';
  line[POS_NEWLINE1] = NEWLINE1;
  line[POS_NEWLINE2] = NEWLINE2;
  line[POS_NULLTERM]  = '\0';

  p = (const unsigned char *)data;

  pos = 0;
  for (count = 0; count < len; count++) {
    pos = count % OCTETS_PER_LINE;

    if (pos == 0) {
      addr_val = addr;
      for (i = 0; i < LEN_ADDRESS; i++) {
        line[POS_ADDRESS + LEN_ADDRESS - i - 1] = hex[addr_val & 0x0f];
        addr_val >>= 4;
      }
      addr += OCTETS_PER_LINE;
    }

    if (pos < OCTET_WITH_DIVIDER) {
      line[GET_POS_HEX_OCTET_L(pos) + 0] = hex[(*p >> 4) & 0x0f];
      line[GET_POS_HEX_OCTET_L(pos) + 1] = hex[*p & 0x0f];
    } else {
      line[GET_POS_HEX_OCTET_H(pos) + 0] = hex[(*p >> 4) & 0x0f];
      line[GET_POS_HEX_OCTET_H(pos) + 1] = hex[*p & 0x0f];
    }

    line[GET_POS_ASCII(pos)] = (IS_PRINTABLE(*p) ? *p : '.');

    p++;

    if (pos == (OCTETS_PER_LINE - 1)) {
      send_line(line, arg);
    }
  }

  if (pos < OCTETS_PER_LINE) {
    for (++pos; pos < OCTETS_PER_LINE; pos++) {
      if (pos < OCTET_WITH_DIVIDER) {
        line[GET_POS_HEX_OCTET_L(pos) + 0] = ' ';
        line[GET_POS_HEX_OCTET_L(pos) + 1] = ' ';
      } else {
        line[GET_POS_HEX_OCTET_H(pos) + 0] = ' ';
        line[GET_POS_HEX_OCTET_H(pos) + 1] = ' ';
      }

      line[GET_POS_ASCII(pos)] = ' ';
    }

    send_line(line, arg);
  }
}


void
hexdump(const void *data, size_t len)
{
  hexdump_std(data, len, 0, 0);
}
