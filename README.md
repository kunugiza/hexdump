# hexdump

## What is this?
Hexdump implemented with no library dependencies including libc.

## Configuration

Implement your own `send_line` function to suit your requirements.
The example implementations goes out to stdout.

If you need CR-LF output then configure these as well.

```
#define NEWLINE1		'\n'
#define NEWLINE2		'\0'
```

## Usage

Just call void `hexdump_std(const void *data, size_t len, size_t addr, void *arg)` with necessary arguments.

data - The pointer of the data to be dumped.

len - Length of the data to be dumped.

addr - Starting address that should be printed as offset with the dump output.

arg - Arbitrary pointer that will be passed to send_line function. Could be something like pointer to stderr.

`void hexdump(const void *data, size_t len)` is provieded also as a shortcut for `hexdump_std(data, len, 0, NULL)`.
