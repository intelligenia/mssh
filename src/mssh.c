#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include <gtk/gtk.h>

#include "config.h"
#include "mssh-window.h"

#define CONFFILE    ".mssh_clusters"
#define PKGINFO     PACKAGE_NAME " " VERSION
#define COPYRIGHT   "Copyright (C) 2009 Bradley Smith <brad@brad-smith.co.uk>"

static void on_mssh_destroy(GtkWidget *widget, gpointer data)
{
    gtk_widget_hide(widget);
    gtk_main_quit();
}

void usage(const char *argv0)
{
    fprintf(stderr, "%s\n", PKGINFO);
    fprintf(stderr, "%s\n", COPYRIGHT);
    fprintf(stderr, "An ssh client to issue the same commands to multiple servers\n\n");
    fprintf(stderr, "Usage: %s [OPTION]... (-a ALIAS | HOSTS)\n\n",
        argv0);
    fprintf(stderr,
        "  -a, --alias=ALIAS    Open hosts associated with named alias\n");
    fprintf(stderr,
        "  -c, --columns=NUM    Override gconf for number of columns\n");
    fprintf(stderr,
        "  -h, --help           Display this help and exit\n");
    fprintf(stderr,
        "  -V, --version        Output version information and exit\n");
    fprintf(stderr, "\nReport bugs to <%s>.\n", PACKAGE_BUGREPORT);
    exit(EXIT_FAILURE);
}

static char *fgetline(FILE *stream)
{
    size_t len = 64;
    size_t pos = 0;
    char c;
    char *buf;

    if((buf = malloc(len)) == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while((c = fgetc(stream)) != EOF)
    {
        if(pos >= len)
        {
            len *= 2;
            if((buf = realloc(buf, len)) == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        if(c == '\n')
        {
            buf[pos++] = '\0';
            break;
        }
        else
        {
            buf[pos++] = c;
        }
    }

    if(c == EOF)
    {
        free(buf);
        return NULL;
    }

    return buf;
}

void append_alias(char *alias, GArray *hosts, GData **aliases, int lineno)
{
    int i;
    GArray *fetched;

    if((fetched = g_datalist_get_data(aliases, alias)) == NULL)
    {
        printf("Line %d: Alias '%s' not defined\n", lineno, alias);
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < fetched->len; i++)
    {
        g_array_append_val(hosts, g_array_index(fetched, char*, i));
    }
}

GData **parse_aliases(char *conffile)
{
    FILE *file;
    char *line;
    int lineno = 0;

    GData **aliases = malloc(sizeof(GData*));
    g_datalist_init(aliases);

    if((file = fopen(conffile, "r")) == NULL)
        return aliases;

    while((line = fgetline(file)) != NULL)
    {
        char *sep, *alias, *hoststr, *tmp;
        GArray *hosts;

        lineno++;

        if(strcmp(line, "") == 0)
            continue;

        if((sep = strchr(line, ':')) == NULL)
        {
            printf("Line %d: Failed to parse line '%s'\n", lineno, line);
            exit(EXIT_FAILURE);
        }

        *sep = '\0';
        alias = line;
        hoststr = sep + 1;

        if((tmp = strtok(hoststr, " ")) == NULL)
        {
            printf("Line %d: Alias '%s' specifies no hosts\n", lineno,
                alias);
            exit(EXIT_FAILURE);
        }

        hosts = g_array_new(FALSE, TRUE, sizeof(char*));

        do
        {
            if(tmp[0] == '[' && tmp[strlen(tmp) - 1] == ']')
            {
                tmp++;
                tmp[strlen(tmp) - 1] = '\0';
                append_alias(tmp, hosts, aliases, lineno);
            }
            else
                g_array_append_val(hosts, tmp);
        }
        while((tmp = strtok(NULL, " ")) != NULL);

        g_datalist_set_data(aliases, alias, hosts);
    }

    return aliases;
}

int main(int argc, char* argv[], char* env[])
{
    GtkWidget* window;
    int c, option_index = 0;
    char *home, *conffile;
    long cols = 0;
    GData **aliases = NULL;
    GArray *hosts = NULL;

    static struct option long_options[] =
    {
        {"alias",   required_argument,  0, 'a'},
        {"columns", required_argument,  0, 'c'},
        {"help",    no_argument,        0, 'h'},
        {"version", no_argument,        0, 'V'},
        {0, 0, 0, 0}
    };

    if((home = getenv("HOME")) != NULL)
    {
        int len = strlen(home) + strlen(CONFFILE) + 2;

        conffile = malloc(len);
        snprintf(conffile, len, "%s/%s", home, CONFFILE);

        aliases = parse_aliases(conffile);
        free(conffile);
    }
    else
    {
        fprintf(stderr,
            "Warning: $HOME not set, not reading config file\n");
    }

    for(;;)
    {
        c = getopt_long(argc, argv, "a:c:hV", long_options, &option_index);

        if(c == -1)
            break;

        switch(c)
        {
        case 'a':
            if(aliases && (hosts = g_datalist_get_data(aliases,
                optarg)) == NULL)
            {
                fprintf(stderr, "Alias '%s' not found\n\n", optarg);
                usage(argv[0]);
            }
            break;
        case 'c':
            errno = 0;
            cols = strtol(optarg, NULL, 10);
            if(cols <= 0 || errno != 0)
            {
                fprintf(stderr, "Invalid number of columns '%s'\n\n",
                    optarg);
                usage(argv[0]);
            }
            break;
        case 'h':
            usage(argv[0]);
            break;
        case 'V':
            printf("%s\n\n", PKGINFO);
            printf("%s\n\n", COPYRIGHT);
            printf("Redistribution and use in source and binary forms, with or without\n");
            printf("modification, are permitted provided that the following conditions are met:\n");
            printf("\n");
            printf("    1. Redistributions of source code must retain the copyright notice,\n");
            printf("       this list of conditions and the following disclaimer.\n");
            printf("    2. Redistributions in binary form must reproduce the copyright notice,\n");
            printf("       this list of conditions and the following disclaimer in the\n");
            printf("       documentation and/or other materials provided with the distribution.\n");
            printf("    3. The name of the author may not be used to endorse or promote\n");
            printf("       products derived from this software without specific prior written\n");
            printf("       permission.\n");
            printf("\n");
            printf("THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR\n");
            printf("IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES\n");
            printf("OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN\n");
            printf("NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n");
            printf("SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED\n");
            printf("TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n");
            printf("PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n");
            printf("LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n");
            printf("NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n");
            printf("SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");

            exit(EXIT_SUCCESS);
            break;
        case '?':
            printf("\n");
            usage(argv[0]);
            exit(EXIT_FAILURE);
            break;
        default:
            abort();
        }
    }

    if(hosts == NULL)
    {
        hosts = g_array_new(FALSE, TRUE, sizeof(char*));
        if (optind < argc)
        {
            while (optind < argc)
            {
                char *host = strdup(argv[optind++]);
                g_array_append_val(hosts, host);
            }
        }
        else
        {
            fprintf(stderr, "No hosts specified\n\n");
            usage(argv[0]);
        }
    }

    gtk_init(&argc, &argv);

    window = GTK_WIDGET(mssh_window_new());

    g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(on_mssh_destroy), NULL);

    mssh_window_start_session(MSSH_WINDOW(window), env, hosts, cols);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
