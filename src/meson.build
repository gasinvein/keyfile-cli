sources = [
  'keyfile.c'
]

keyfile_h = configuration_data()
keyfile_h.set('PROJECT_NAME', meson.project_name())
configure_file(output : 'config.h',
               configuration: keyfile_h)

keyfile_cli = executable(
  'keyfile',
  sources,
  dependencies: [glib_dep],
  install : true)
