/* $id$
 *
 * Internet Radio Locator for GTK 4
 *
 * Copyright (C) 2014, 2015, 2016, 2017, 2018, 2022, 2025  Aamot Research
 *
 * Author: Ole Aamot <ole@aamot.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "gtk-internet-radio-locator.h"
#include "gtk-internet-radio-locator-station.h"

extern GTKInternetRadioLocatorData *gtk_internet_radio_locator;
extern GTKInternetRadioLocatorStationInfo *stationinfo, *localstation;

extern gchar *world_station_xml_filename, *local_station_xml_file;

extern GStatBuf stats;

extern GtkEntryCompletion *completion;
GtkListStore *model = NULL;
GtkTreeIter iter;

void gtk_internet_radio_locator_helper_run(gchar *url, gchar *name, GTKInternetRadioLocatorStreamType type, GTKInternetRadioLocatorHelperType helper)
{
	GError *err = NULL;
	GTimeVal mtime;
	/* const char *mime_info = NULL; */
	/* GtkVFSMimeApplication *app; */
	char *app = NULL, *command = NULL, *msg = NULL;
	char **argv = NULL;
	gint argc;
	gboolean    ret;
	GError *error = NULL;
	GCancellable *player_cancellable;
	GCancellable *record_cancellable;
	
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror(0);
		exit(1);
	}
	
	g_return_if_fail(url != NULL);
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("%s", url);

	/* mime_info = gtk_vfs_get_mime_type(url); */

	g_get_current_time(&mtime);

	/* app = (gchar *)gtk_vfs_mime_get_default_application (mime_info); */

	// gtk_internet_radio_locator_player_main(url, gtk_internet_radio_locator->selected_station_name);

	if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
		/* gtk_internet_radio_locator_player_main(url, gtk_internet_radio_locator->selected_station_name); */
	}

	if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {
		/* gtk_internet_radio_locator_record_main(url, gtk_internet_radio_locator->selected_station_name); */
	}

	return;

#if 0
	if (g_strcmp0(app,"no")!=0) {
		command = g_strconcat(app, " ", url, NULL);
		GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Helper application is %s\n", command);
		if (type == GTK_INTERNET_RADIO_LOCATOR_STREAM_SHOUTCAST) {
			if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
				command = g_strconcat(app, " ", url, NULL);
				/* argv[0] = g_strdup(app); */
				/* argv[1] = g_strdup(url); */
			}
			if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {

				GtkWidget *dialog;
				char *filename, *default_filename;
				GDate gdate;
				time_t t;
				struct tm *tmp;
				char outstr[200];
				t = time(NULL);
				tmp = localtime(&t);
				strftime(outstr, sizeof(outstr), "%a, %d %b %Y %T %z", tmp);
				dialog = gtk_file_chooser_dialog_new ("Save Recorded Broadcast File",
								      GTK_WINDOW(gtk_internet_radio_locator_app),
								      GTK_FILE_CHOOSER_ACTION_SAVE,
								      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
								      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
								      NULL);
				default_filename = g_strconcat(gtk_internet_radio_locator->selected_station_name, " - Broadcast Recording - ", outstr, ".mp3", NULL);  
				gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), default_filename);
				if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
					gtk_internet_radio_locator->selected_archive_file = g_strconcat(filename, NULL);
				}
				gtk_widget_destroy (dialog);
				
				/* printf("Archiving program at %s\n", archive); */
				
				appbar_send_msg(_("Recording from %s in %s to %s"),
						gtk_internet_radio_locator->selected_station_name,
						gtk_internet_radio_locator->selected_station_location,
						gtk_internet_radio_locator->selected_archive_file);		
				
				command = g_strconcat(app, " ", url, " -d ", g_get_home_dir(), "/.internet_radio_locator/ -D %D", NULL);
				GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("%s\n", command);

				// gtk_internet_radio_locator_archive_new("Archive", gtk_internet_radio_locator->selected_archive_file);
				
				/* " -d ", g_get_home_dir(), "/.gtk_internet_radio_locator -D \"", name, "\" -s -a -u gtk_internet_radio_locator/", VERSION, NULL); */
				/* command = g_strconcat(command, " -d ", g_get_home_dir(), "/.internet_radio_locator/", name, " -D %S%A%T -t 10 -u internet_radio_locator/", VERSION, NULL); */
			}
		        GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Helper command is %s\n", command);
		}
		/* gtk_vfs_mime_application_free (app); */
	} else {
		if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
			msg = g_strdup_printf(_("An error happened trying to play "
						"%s\nEither the file doesn't exist, or you "
						"don't have a player for it."),
					      url);
		}
		if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {
			msg = g_strdup_printf(_("An error happened trying to record "
						"%s\nEither the file doesn't exist, or you "
						"don't have a recorder for it."),
					      url);
		}
		if (msg != NULL) {
			show_error(msg);
			g_free(msg);
		}
		return;
	}

	if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
#if 0
		g_shell_parse_argv(command,
				   &argc,
				   &argv,
				   NULL);
		ret = g_spawn_async_with_pipes (".",
						argv,
						NULL,
						G_SPAWN_SEARCH_PATH|G_SPAWN_STDOUT_TO_DEV_NULL|G_SPAWN_STDERR_TO_DEV_NULL|G_SPAWN_DO_NOT_REAP_CHILD,
						NULL,
						NULL,
						&gtk_internet_radio_locator->player_pid,
						NULL,
						NULL,
						NULL,
						&err);
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gtk_internet_radio_locator->player_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gtk_internet_radio_locator->player_pid, (GChildWatchFunc)cb_child_watch_player, gtk_internet_radio_locator);
		
		/* gtk_internet_radio_locator->player_pid = pid; */
		gtk_internet_radio_locator->player_status = GTK_INTERNET_RADIO_LOCATOR_PLAYER_TRUE;
		/* Install timeout fnction that will move the progress bar */
		gtk_internet_radio_locator->timeout_id = g_timeout_add(100,(GSourceFunc)cb_timeout,gtk_internet_radio_locator);
/* #endif */
/* #if 0 */
		ret = g_spawn_async_with_pipes( NULL, /* command */ argv, NULL,
						G_SPAWN_DO_NOT_REAP_CHILD|G_SPAWN_DEFAULT, NULL,
						NULL, &gtk_internet_radio_locator->player_pid, NULL, &out, &error, NULL );
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gtk_internet_radio_locator->player_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gtk_internet_radio_locator->player_pid, (GChildWatchFunc)cb_child_watch_player, gtk_internet_radio_locator );
		/* Create channels that will be used to read gtk_internet_radio_locator from pipes. *
			
#ifdef G_OS_WIN32
		out_ch = g_io_channel_win32_new_fd( out );
		err_ch = g_io_channel_win32_new_fd( error );
#else
		out_ch = g_io_channel_unix_new( out );
		err_ch = g_io_channel_unix_new( error );
#endif
		*/

                /* Add watches to channels */
		g_io_add_watch( out_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_out_watch, gtk_internet_radio_locator );
		g_io_add_watch( err_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_err_watch, gtk_internet_radio_locator );
		/* Install timeout fnction that will move the progress bar */
		gtk_internet_radio_locator->timeout_id = g_timeout_add( 100, (GSourceFunc)cb_timeout, gtk_internet_radio_locator );
/* #endif */
#if 0
		if (!g_spawn_command_line_sync(command, stdout, stderr, status, &err)) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Status code: %i\n"
						"Details: %s"), url, status, err->message);
			show_error(msg);
			g_error_free(err);
			g_free(msg);
		} else {
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s\n", command);
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gtk_internet_radio_locator->player_pid, (GChildWatchFunc)cb_child_watch_player, gtk_internet_radio_locator );
#endif
		/* Original async player code */

		gtk_internet_radio_locator->player_launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
		gtk_internet_radio_locator->player_subprocess = g_subprocess_launcher_spawn (gtk_internet_radio_locator->player_launcher, &error, app, url, NULL);
		if (gtk_internet_radio_locator->player_subprocess == NULL) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Details: %s"), url, error->message);
			show_error(msg);
			g_error_free(error);
			g_free(msg);
			goto quit_player;
		} else {
			gtk_internet_radio_locator->player_status = GTK_INTERNET_RADIO_LOCATOR_PLAYER_TRUE;
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s player\n", command);
		}

#endif
#if 0
		if (!g_spawn_command_line_async(command, &err)) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Details: %s"), url, err->message);
			show_error(msg);
			g_error_free(err);
			g_free(msg);
		} else {
			gtk_internet_radio_locator->player_status = GTK_INTERNET_RADIO_LOCATOR_PLAYER_TRUE;
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s player\n", command);
			g_subprocess_wait_async (gtk_internet_radio_locator->player_subprocess,
						 player_cancellable,
						 (GAsyncReadyCallback)cb_subprocess_player,
						 NULL);
		}
#endif

	}
	
	if (helper == GTK_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {
		
/* #if 0 */
		/* gchar *argv[] = { command, NULL }; */
		/* Spawn child process */
		gchar *recording_path_name = g_strconcat(g_get_home_dir(), "/.internet_radio_locator/", NULL);
		gchar *formatting_argument = g_strdup("-D %D");
		gtk_internet_radio_locator->record_launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
		/* gtk_internet_radio_locator->record_subprocess = g_subprocess_launcher_spawn (gtk_internet_radio_locator->record_launcher, &error, GTK_INTERNET_RADIO_LOCATOR_HELPER_RECORD, url, "-d", recording_path_name, "-D", "%D", NULL); */
		if (gtk_internet_radio_locator->record_subprocess == NULL) {
			/* msg = g_strdup_printf(_("Failed to run '%s'\n" */
			/* 			"Details: %s"), GTK_INTERNET_RADIO_LOCATOR_HELPER_RECORD, error->message); */
			show_error(msg);
			g_error_free(error);
			g_free(msg);
			goto quit_record;
		} else {
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s\n", command);
			g_subprocess_wait_async (gtk_internet_radio_locator->record_subprocess,
						 record_cancellable,
						 (GAsyncReadyCallback)cb_subprocess_record, NULL);
		}

#if 0
		g_shell_parse_argv(command,
				   &argc,
				   &argv,
				   NULL);
		ret = g_spawn_async_with_pipes (".",
						argv,
						NULL,
						G_SPAWN_SEARCH_PATH|G_SPAWN_STDOUT_TO_DEV_NULL|G_SPAWN_STDERR_TO_DEV_NULL|G_SPAWN_DO_NOT_REAP_CHILD,
						NULL,
						NULL,
						&gtk_internet_radio_locator->record_pid,
						NULL,
						NULL,
						NULL,
						&err);
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gtk_internet_radio_locator->record_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gtk_internet_radio_locator->record_pid, (GChildWatchFunc)cb_child_watch_record, gtk_internet_radio_locator);

		/* gtk_internet_radio_locator->record_pid = pid; */
		gtk_internet_radio_locator->record_status = GTK_INTERNET_RADIO_LOCATOR_RECORD_TRUE;

		gtk_internet_radio_locator_archive_new("Archive", gtk_internet_radio_locator->selected_archive_file, gtk_internet_radio_locator->selected_streams_codec);
				
		/* Install timeout fnction that will move the progress bar */
		gtk_internet_radio_locator->timeout_id = g_timeout_add(100,(GSourceFunc)cb_timeout,gtk_internet_radio_locator);
/* #endif */
#if 0 
		ret = g_spawn_async_with_pipes( NULL, /* command */ argv, NULL,
						G_SPAWN_DO_NOT_REAP_CHILD|G_SPAWN_DEFAULT, NULL,
						NULL, &gtk_internet_radio_locator->record_pid, NULL, &out, &error, NULL );
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gtk_internet_radio_locator->record_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gtk_internet_radio_locator->record_pid, (GChildWatchFunc)cb_child_watch_record, gtk_internet_radio_locator );
		/* Create channels that will be used to read gtk_internet_radio_locator from pipes. */
#ifdef G_OS_WIN32
		out_ch = g_io_channel_win32_new_fd( out );
		err_ch = g_io_channel_win32_new_fd( error );
#else
		out_ch = g_io_channel_unix_new( out );
		err_ch = g_io_channel_unix_new( error );
#endif
		/* Add watches to channels */
		g_io_add_watch( out_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_out_watch, gtk_internet_radio_locator );
		g_io_add_watch( err_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_err_watch, gtk_internet_radio_locator );
		/* Install timeout fnction that will move the progress bar */
		gtk_internet_radio_locator->timeout_id = g_timeout_add( 100, (GSourceFunc)cb_timeout, gtk_internet_radio_locator );
#endif
#if 0
		if (!g_spawn_command_line_sync(command, stdout, stderr, status, &err)) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Status code: %i\n"
						"Details: %s"), url, status, err->message);
			show_error(msg);
			g_error_free(err);
			g_free(msg);
		} else {
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s\n", command);
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gtk_internet_radio_locator->record_pid, (GChildWatchFunc)cb_child_watch_record, gtk_internet_radio_locator );
#endif
#endif
	}

quit_player:

        if (gtk_internet_radio_locator->player_subprocess)
		g_object_unref(gtk_internet_radio_locator->player_subprocess);
        if (gtk_internet_radio_locator->player_launcher)
		g_object_unref(gtk_internet_radio_locator->player_launcher);
quit_record:
        if (gtk_internet_radio_locator->record_subprocess)
		g_object_unref(gtk_internet_radio_locator->record_subprocess);
        if (gtk_internet_radio_locator->record_launcher)
		g_object_unref(gtk_internet_radio_locator->record_launcher);
#endif
}

static void
gtk_internet_radio_locator_station_parser(GTKInternetRadioLocatorStationInfo * station, xmlDocPtr doc,
		    xmlNodePtr cur)
{
	xmlNodePtr sub;
	char *chans;

	g_return_if_fail(station != NULL);
	g_return_if_fail(doc != NULL);
	g_return_if_fail(cur != NULL);

	station->id = (gchar *)xmlGetProp(cur, (const xmlChar *)"id");
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->id = %s\n", station->id);
	station->name = (gchar *)xmlGetProp(cur, (const xmlChar *)"name");
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->name = %s\n", station->name);
	station->rank = (gchar *)xmlGetProp(cur, (const xmlChar *)"rank");
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->rank = %s\n", station->rank);
	station->type = (gchar *)xmlGetProp(cur, (const xmlChar *)"type");
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->type = %s\n", station->type);
	station->band = (gchar *)xmlGetProp(cur, (const xmlChar *)"band");
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->band = %s\n", station->band);

	sub = cur->xmlChildrenNode;

	while (sub != NULL) {

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "frequency"))) {
			station->frequency = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->frequency = %s\n",
			    station->frequency);
		}

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "location"))) {
			station->location = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->location = %s\n", station->location);

		}

		if ((!xmlStrcmp
		     (sub->name, (const xmlChar *) "description"))) {
			station->description = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->description = %s\n", station->description);
		}

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "uri"))) {
			station->uri = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->uri = %s\n", station->uri);
			/* fprintf(stdout, "<a href=\"%s\">%s (%s)</a>\n\n", station->uri, station->name, station->location); */
		}

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "stream"))) {

			GTKInternetRadioLocatorStreamInfo *stream = g_new0(GTKInternetRadioLocatorStreamInfo, 1);

			/* gtk_internet_radio_locator->stream_count++; */

			station->stream = stream;

			station->stream->mimetype = (gchar *)
				xmlGetProp(sub, (const xmlChar *)"mime");
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->mimetype = %s\n",
			    station->stream->mimetype);
			if (xmlGetProp(sub, (const xmlChar *)"bitrate") != NULL) {
				station->stream->bitrate =
					(glong)atol((char *)xmlGetProp(sub, (const xmlChar *)"bitrate"));
				GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->bitrate = %li\n",
				    station->stream->bitrate);
			}

			if (xmlGetProp(sub, (const xmlChar *)"samplerate") != NULL) {
				station->stream->samplerate = (glong) atol((char *)xmlGetProp(sub, (const xmlChar *)"samplerate"));
			}

			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->samplerate = %li\n",
			    station->stream->samplerate);
			station->stream->uri = (gchar *)xmlGetProp(sub, (const xmlChar *)"uri");
			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->uri = %s\n",
			    station->stream->uri);

			chans = (gchar *)xmlGetProp(sub, (const xmlChar *)"stations");

			if (chans != NULL) {
				if (strcmp(chans, "stereo") == 0) {
					station->stream->channels =
					    GTK_INTERNET_RADIO_LOCATOR_CHANNELS_STEREO;
					GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->channels = %d\n", station->stream->channels);
				} else if (strcmp(chans, "mono") == 0) {
					station->stream->channels =
					    GTK_INTERNET_RADIO_LOCATOR_CHANNELS_MONO;
					GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->channels = %d\n", station->stream->channels);
				} else if (strcmp(chans, "5:1") == 0) {
					station->stream->channels =
					    GTK_INTERNET_RADIO_LOCATOR_CHANNELS_5_1;
					GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->channels = %d\n", station->stream->channels);
				}
				g_free(chans);
			}

		}
		/* if... "stream" */
		sub = sub->next;
	}
	/* gtk_internet_radio_locator->station_count++; */
	return;
}

GTKInternetRadioLocatorStationInfo *gtk_internet_radio_locator_station_load_from_http(GTKInternetRadioLocatorStationInfo * head,
					     gpointer data)
{
	GTKInternetRadioLocatorStationInfo *gstation;
	gstation = gtk_internet_radio_locator_station_load_from_file (head, "https://www.gnome.org/~ole/gtk-internet-radio-locator/internet-radio-locator.xml");
	return gstation;
}

GTKInternetRadioLocatorStationInfo *gtk_internet_radio_locator_station_load_from_file(GTKInternetRadioLocatorStationInfo * head,
					     char *filename)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;
	GTKInternetRadioLocatorStationInfo *curr = NULL;
	char *version;
	GTKInternetRadioLocatorStationInfo *mem_station;

	g_return_val_if_fail(filename != NULL, NULL);

	doc = xmlReadFile(filename, NULL, 0);

	if (doc == NULL) {
		perror("xmlParseFile");
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr, "Empty document\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "internet_radio_locator")) {
		fprintf(stderr,
			"Document of wrong type, root node != internet_radio_locator\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	version = (gchar *)xmlGetProp(cur, (const xmlChar *)"version");

	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Valid GTKInternetRadioLocator %s XML document... Parsing stations...\n",
	    version);

	free(version);

	cur = cur->xmlChildrenNode;

	while (cur != NULL) {

		if ((!xmlStrcmp(cur->name, (const xmlChar *) "station"))) {

			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Found a new station.\n");

			curr = g_new0(GTKInternetRadioLocatorStationInfo, 1);
			mem_station = g_new0(GTKInternetRadioLocatorStationInfo, 1);

			gtk_internet_radio_locator_station_parser(curr, doc, cur);

			curr->next = head;

			head = curr;

			mem_station = head;

			gtk_internet_radio_locator_stations = g_list_append(gtk_internet_radio_locator_stations, (GTKInternetRadioLocatorStationInfo *)mem_station);

			GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Done with parsing the station.\n");

		}
		cur = cur->next;
	}

	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Finished parsing XML document.\n");

	xmlFreeDoc(doc);

	return curr;
}

gint gtk_internet_radio_locator_station_update (GTKInternetRadioLocatorStationInfo *head, gchar *station_band, gchar *station_description, gchar *station_name, gchar *station_location, gchar *station_uri, gchar *station_website) {
	gchar msg[256];
	gchar *msg1, *msg2;
	/* Open ~/.gtk_internet_radio_locator/gtk_internet_radio_locator.xml.  Parse structure.  Insert new item.  Save structure. */
	GTKInternetRadioLocatorStationInfo *new_station;
	GTKInternetRadioLocatorStationInfo *stationinfo;
	/* GList *gtk_internet_radio_locator_local_stations = NULL; */
	gchar *local_station_uri, *local_station_name, *local_station_location, *local_station_band, *local_station_description, *local_station_website;
	gchar *stations = g_strconcat(g_get_home_dir(), "/.internet-radio-locator/internet-radio-locator.xml", NULL);

	gboolean local_gtk_internet_radio_locator_file = g_file_test (".internet-radio-locator/internet-radio-locator.xml", G_FILE_TEST_EXISTS);

	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("local_gtk_internet_radio_locator_file = %i\n", local_gtk_internet_radio_locator_file);

	if (local_gtk_internet_radio_locator_file == 0) {

		gchar *local_gtk_internet_radio_locator_directory = g_strconcat(g_get_home_dir(), "/.internet-radio-locator", NULL);
		g_mkdir_with_parents (local_gtk_internet_radio_locator_directory, 0700);
		
	}
	
	FILE *fp;
	
	stationinfo = gtk_internet_radio_locator_station_load_from_file(NULL, stations);

	new_station = g_new0(GTKInternetRadioLocatorStationInfo, 1);
	new_station->name = g_strdup(station_name);
	new_station->band = g_strdup(station_band);
	new_station->description = g_strdup(station_description);
	new_station->location = g_strdup(station_location);
	new_station->stream = g_new0(GTKInternetRadioLocatorStreamInfo, 1);
	new_station->stream->uri = g_strdup(station_uri);
	new_station->uri = g_strdup(station_website);
	fp = g_fopen(stations, "w+");
	g_fprintf(fp, "<?xml version='1.0' encoding='UTF-8'?>\n<!DOCTYPE internet_radio_locator SYSTEM 'internet_radio_locator-1.5.dtd'>\n<internet_radio_locator version='%s'>\n", VERSION);
	// stationinfo-> = l->data;
	while (stationinfo != NULL) {
		local_station_uri = g_strdup(stationinfo->stream->uri);
		local_station_name = g_strdup(stationinfo->name);
		local_station_location = g_strdup(stationinfo->location);
		local_station_band = g_strdup(stationinfo->band);
		local_station_description = g_strdup(stationinfo->description);
		local_station_website = g_strdup(stationinfo->uri);
		/* FIXME: Save mime='audio/mp3' uri='%s' codec='MPEG 1 Audio, Layer 3 (MP3)' samplerate='24000 Hz' channels='Mono' bitrate='32 kbps' */
		g_fprintf(fp, "  <station band=\"%s\" id=\"%s\" lang=\"en\" name=\"%s\" rank=\"1.0\" type=\"org\">\n    <frequency uri=\"%s\">%s in %s</frequency>\n    <location>%s</location>\n    <description lang=\"en\">%s</description>\n    <stream uri=\"%s\" />\n    <uri>%s</uri>\n  </station>\n", local_station_band, local_station_name, local_station_name, local_station_website, local_station_band, local_station_location, local_station_location, local_station_description, local_station_uri, local_station_website);
		stationinfo = stationinfo->next;

	}
	g_fprintf(fp, "  <station band=\"%s\" id=\"%s\" lang=\"en\" name=\"%s\" rank=\"1.0\" type=\"org\">\n    <frequency uri=\"%s\">%s in %s</frequency>\n    <location>%s</location>\n    <description lang=\"en\">%s</description>\n    <stream uri=\"%s\" />\n    <uri>%s</uri>\n  </station>\n", new_station->band, new_station->name, new_station->name, new_station->uri, new_station->band, new_station->location, new_station->location, new_station->description, new_station->stream->uri, new_station->uri);
	g_fprintf(fp, "</internet_radio_locator>\n");
	fclose(fp);
	gtk_internet_radio_locator_stations = g_list_append(gtk_internet_radio_locator_stations, (GTKInternetRadioLocatorStationInfo *)new_station);

	g_free(stations);
	g_free(new_station);
	g_free(stationinfo);

	model = gtk_list_store_new(11, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	world_station_xml_filename = g_strconcat(GTK_INTERNET_RADIO_LOCATOR_DATADIR, "/internet-radio-locator.xml", NULL);
	GTK_INTERNET_RADIO_LOCATOR_DEBUG_MSG("world_station_xml_filename = %s\n",
	    world_station_xml_filename);

	if (world_station_xml_filename == NULL) {
		g_warning(("Failed to open %s.  Please install it.\n"),
			  world_station_xml_filename);
	}

	local_station_xml_file =
	    g_strconcat(g_get_home_dir(), "/.internet-radio-locator/internet-radio-locator.xml", NULL);

	if (!g_stat(local_station_xml_file, &stats)) {
		localstation = gtk_internet_radio_locator_station_load_from_file(NULL, local_station_xml_file);
	} else {
		localstation = NULL;
	}

	if (localstation == NULL) {
		printf("Failed to open %s.\n", local_station_xml_file);
	}

/*   g_free (local_station_xml_file); */

	stationinfo = gtk_internet_radio_locator_station_load_from_file(localstation, world_station_xml_filename);

	gtk_internet_radio_locator_stations = NULL;

	while (stationinfo != NULL) {

		gtk_list_store_append(model, &iter);
		gtk_list_store_set(model,
				   &iter,
				   STATION_NAME,
				   stationinfo->name,
				   STATION_LOCATION,
				   stationinfo->location,
				   STATION_URI,
				   stationinfo->stream->uri,
				   STATION_DESCRIPTION,
				   stationinfo->description,
				   STATION_FREQUENCY,
				   stationinfo->frequency,
				   STATION_BAND,
				   stationinfo->band,
				   STATION_TYPE,
				   stationinfo->type,
				   STATION_RANK,
				   stationinfo->rank,
				   STATION_BITRATE,
				   stationinfo->bitrate,
				   STATION_SAMPLERATE,
				   stationinfo->samplerate,
				   STATION_ID,
				   stationinfo->id,
				   -1);

		stationinfo = stationinfo->next;
	}

	gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(model));

	return (0);
}
