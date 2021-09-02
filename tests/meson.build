sample = files('sample.ini')

test('List groups', keyfile_cli,
     args: [sample])
test('List keys in group', keyfile_cli,
     args: [sample, '-g', 'Desktop Entry'])
test('Non-existent group', keyfile_cli,
     args: [sample, '-g', 'Desktop Action no-such-action'],
     should_fail: true)
test('Read string value', keyfile_cli,
     args: [sample, '-g', 'Desktop Entry', '-k', 'Exec'])
test('Read list value', keyfile_cli,
     args: [sample, '-g', 'Desktop Entry', '-k', 'Keywords', '-l'])
test('Add new value', keyfile_cli,
     args: [sample, '-g', 'Desktop Entry', '-k', 'Keywords', '-a', 'another-one'],)
test('Non-existent key in group', keyfile_cli,
     args: [sample, '-g', 'Desktop Action some-action', '-k', 'Icon'],
     should_fail: true)
test('Set new value', keyfile_cli,
     args: [sample, '-g', 'Desktop Action new-action', '-k', 'Keywords', '-s', 'one'],)
