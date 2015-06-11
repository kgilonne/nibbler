Nibbler is a command line tool for print different DNS records in a formated JSON output.

**Example**: `./nibbler example.com`

INSTALLATION
------------

The current version has been compiled on:
- Debian GNU/Linux 7.7 (gcc version 4.7.2).
- MAC OS X 10.9.5 (clang-600.0.54)

Use the command `make`.
This will install c-ares library and nibbler.

C-ARES INSTALLATION
-------------------

Library github can be found [here](https://github.com/bagder/c-ares)

In one line:
```sh
$ tar -xf ./lib/c-ares-1.10.0.tar.gz; cd ./c-ares-1.10.0; ./configure --prefix=$PWD/../ --disable-shared; make; make install; cd ..; rm -r ./c-ares-1.10.0/
```

Step by step:
```sh
$ tar -xf ./lib/c-ares-1.10.0.tar.gz
$ cd ./c-ares-1.10.0
$ ./configure --prefix=$PWD/../ --disable-shared
$ make
$ make install
$ cd ..
$ rm -r ./c-ares-1.10.0/
```

Or simply `make reinstall_c-ares`.

DKIM RECORD
-----------

```
$./nibbler -dkim example example.com
```

This will make a TXT request with the specified selector.
In the example above, it will be `example._domain_key.mailjet.com`.

SPF CHECK
---------

```
./nibbler -spf example.com other.com
```

This option need to install spfquery binary from [libspf2](http://www.libspf2.org/).
> TODO: Use the library's function instead.

EXIT_STATUS
-----------
	-1 if error result from system call fail,
	0 if OK,
	1 if the domain name don't exist,
	2 if you need to try again.
