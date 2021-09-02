#include <locale.h>

#include <glib.h>

static gchar *opt_group;
static gchar *opt_key;
static gboolean opt_list_value;
static gchar *opt_set_value;
static gchar *opt_add_value;

static GOptionEntry entries[] = {
    { "group", 'g', 0, G_OPTION_ARG_STRING, &opt_group, "Use this group", "GROUP" },
    { "key", 'k', 0, G_OPTION_ARG_STRING, &opt_key, "Use this key in specified GROUP", "KEY" },
    { "set-value", 's', 0, G_OPTION_ARG_STRING, &opt_set_value, "Set the value for KEY in GROUP to VALUE", "VALUE" },
    { "add-value", 'a', 0, G_OPTION_ARG_STRING, &opt_add_value, "Add item VALUE to the list for KEY in GROUP", "VALUE" },
    { "list", 'l', 0, G_OPTION_ARG_NONE, &opt_list_value, "Specified KEY contains a list", NULL },
    { NULL }
};

static int usage(GOptionContext *context, const char *message)
{
  g_autofree gchar *help = g_option_context_get_help(context, TRUE, NULL);
  g_printerr ("%s\n\n%s", message, help);
  return 1;
}

int print_string_list(gchar **list, gsize length) {
    unsigned long i = 0;
    for (i = 0; i <= length && list[i] != NULL; i++) {
        g_print("%s\n", list[i]);
    }
    return 0;
}

int main(int argc, char **argv) {
    GError *error = NULL;
    GOptionContext *context;
    GKeyFile *keyfile;
    gchar *path;
    int flags = G_KEY_FILE_NONE;

    setlocale(LC_ALL, "");

    context = g_option_context_new("KEYFILE - read ini-like keyfile");
    g_option_context_add_main_entries(context, entries, NULL);

    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        return usage(context, error->message);
    }

    if (argc == 1) {
        return usage(context, "KEYFILE argument is required");
    }
    path = argv[1];

    keyfile = g_key_file_new();
    if (!g_key_file_load_from_file(keyfile, path, flags, &error)) {
        g_printerr("%s\n", error->message);
        return 1;
    }

    // List groups if no options given
    if (opt_group == NULL && opt_key == NULL) {
        gchar **groups;
        gsize n_groups = 0;
        groups = g_key_file_get_groups(keyfile, &n_groups);
        return print_string_list(groups, n_groups);
    }

    // List keys in group if only --group specified
    if (opt_group != NULL && opt_key == NULL) {
        gchar **keys = NULL;
        gsize n_keys = 0;
        keys = g_key_file_get_keys(keyfile, opt_group, &n_keys, &error);
        if (keys == NULL) {
            g_printerr("%s\n", error->message);
            return 1;
        }
        return print_string_list(keys, n_keys);
    }

    if (opt_group == NULL || opt_key == NULL) {
        return usage(context, "Both --group and --key must be specified");
    }

    // Set or add value
    if (opt_set_value != NULL && opt_add_value != NULL)
        return usage(context, "--set-value and --add-value are mutually exclusive");
    if (opt_set_value != NULL || opt_add_value != NULL) {
        if (opt_set_value != NULL)
            g_key_file_set_string(keyfile, opt_group, opt_key, opt_set_value);
        else if (opt_add_value != NULL)
        {
            gchar **values;
            gsize n_values = 0;
            values = g_key_file_get_string_list(keyfile, opt_group, opt_key, &n_values, &error);
            if (values == NULL) {
                g_printerr("%s\n", error->message);
                return 1;
            }
            values[n_values++] = opt_add_value;
            g_key_file_set_string_list(keyfile, opt_group, opt_key,
                                       (const gchar * const *) values, n_values);
        }
        // Save modified keyfile
        if (!g_key_file_save_to_file(keyfile, path, &error))
        {
            g_printerr("%s\n", error->message);
            return 1;
        }
        return 0;
    }

    // Print each string in list value
    if (opt_list_value) {
        gchar **list_value;
        gsize n_values = 0;
        list_value = g_key_file_get_string_list(keyfile, opt_group, opt_key, &n_values, &error);
        if (list_value == NULL) {
            g_printerr("%s\n", error->message);
            return 1;
        }
        return print_string_list(list_value, n_values);
    // Print value as a string
    } else  {
        gchar *str_value;
        str_value = g_key_file_get_string(keyfile, opt_group, opt_key, &error);
        if (str_value == NULL) {
            g_printerr("%s\n", error->message);
            return 1;
        }
        g_print("%s\n", str_value);
        return 0;
    } 

    return usage(context, "Unexpected combination of options");
}
