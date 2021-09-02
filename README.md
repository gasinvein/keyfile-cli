# keyfile-cli

This is a tiny utility for reading INI-like keyfiles, primarilly intended run within shell scripts.

## Usage

One positional argument is always required: path to the keyfile.

- Without any options, list groups in keyfile
- With `-g`/`--group` option, list keys in given group
- With `-g`/`--group` and `-k`/`--key` options, read the value from specified group/key
  - Use `-l`/`--list` to print one list item per line if the key contains a `;`-separated list
