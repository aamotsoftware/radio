/* $Id$
 *
 * GNOME Radio
 *
 * Copyright (C) 2021  Aamot Software
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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/stat.h>

#include <config.h>
#include <gtk/gtk.h>
#include <gtk/gtkcombobox.h>
#include <gst/player/player.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include "gnome-radio.h"
#include "gnome-radio-gui.h"
#include "gnome-radio-keys.h"
#include "gnome-radio-player.h"
#include "gnome-radio-program.h"
#include "gnome-radio-station.h"
#include "gnome-radio-streams.h"
#include "gnome-radio-tz.h"

extern GtkWidget *gnome_radio_app;
extern GtkWidget *search_selector;
extern GtkWidget *input;
extern GtkWidget *statusbar;
extern GstPlayer *player;

GNOMERadioStationInfo *stationdata, *localstation;

GtkWidget *create_stations_selector(char *selected_station_uri,
				    char *filename)
{
	GtkWidget *stations_selector;
	GtkWidget *align, *menu, *drop_down, *item;
	gchar *station_uri, *station_name, *station_location, *station_band, *station_description, *station_website;
	gchar *label, *world_station_xml_filename, *local_station_xml_file;
	int i = 0, selection = -1;
	GStatBuf stats;
	memset(&stats, 0, sizeof(stats));
	/* The Stations dialog */
	/* stations_selector = gtk_dialog_new_with_buttons("Select a station", */
	/* 						GTK_WINDOW(gnome_radio_app), */
	/* 						0,	/\* flags *\/ */
	/* 						NULL, */
	/* 						GTK_RESPONSE_ACCEPT, */
	/* 						NULL); */
	/* gtk_container_set_border_width */
	/*     (GTK_CONTAINER(GTK_DIALOG(stations_selector)->vbox), 6); */

	/* align = gtk_alignment_new(0.5, 0.5, 0, 0); */
	/* gtk_container_add(GTK_CONTAINER */
	/* 		  (GTK_DIALOG(stations_selector)->vbox), align); */
	/* gtk_container_set_border_width(GTK_CONTAINER(align), 6); */
	/* gtk_widget_show(align); */
	menu = gtk_menu_new();
	gtk_widget_show(menu);
	/* creating the menu items */
	/* world_station_xml_filename = gnome_program_locate_file(NULL, */
	/* 						       GNOME_FILE_DOMAIN_APP_DATADIR, */
	/* 						       "gnome-radio/gnome-radio.xml", */
	/* 						       FALSE, */
	/* 						       NULL); */

	/* world_station_xml_filename = g_strdup("https://people.gnome.org/~ole/gnome-radio/gnome-radio.xml"); */
	world_station_xml_filename = g_strconcat(GNOME_RADIO_DATADIR, "/gnome-radio.xml", NULL);
	GNOME_RADIO_DEBUG_MSG("world_station_xml_filename = %s\n",
	    world_station_xml_filename);
	if (world_station_xml_filename == NULL) {
		g_warning(_("Failed to open %s\n"),
			  world_station_xml_filename);
	}
	local_station_xml_file =
	    g_strconcat(g_get_home_dir(), "/.gnome-radio/gnome-radio.xml", NULL);

	if (!g_stat(local_station_xml_file, &stats)) {
		localstation = gnome_radio_station_load_from_file(NULL, local_station_xml_file);
	} else {
		localstation = NULL;
	}
	if (localstation == NULL) {
		g_warning(_("Failed to open %s\n"), local_station_xml_file);
	}
/*   g_free (local_station_xml_file); */
	stationdata =
	    gnome_radio_station_load_from_file(localstation,
					world_station_xml_filename);
	gnome_radio_stations = NULL;
	while (stationdata != NULL) {

		label =
		    g_strconcat(stationdata->name, " (",
				stationdata->location, ")", NULL);
		station_uri = g_strdup(stationdata->stream->uri);
		station_name = g_strdup(stationdata->name);
		station_location = g_strdup(stationdata->location);
		station_band = g_strdup(stationdata->band);
		station_description = g_strdup(stationdata->description);
		station_website = g_strdup(stationdata->uri);
		
		gnome_radio_stations = g_list_append(gnome_radio_stations,(GNOMERadioStationInfo *)stationdata);

		if (label != NULL) {
			item = gtk_menu_item_new_with_label(label);
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
			g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK
					 (on_stations_selector_changed),
					 NULL);
			g_object_set_data(G_OBJECT(item), "station_uri",
					  (gpointer) station_uri);
			g_object_set_data(G_OBJECT(item), "station_name",
					  (gpointer) station_name);
			g_object_set_data(G_OBJECT(item),
					  "station_location",
					  (gpointer) station_location);
			g_object_set_data(G_OBJECT(item),
					  "station_band",
					  (gpointer) station_band);
			g_object_set_data(G_OBJECT(item),
					  "station_description",
					  (gpointer) station_description);
			g_object_set_data(G_OBJECT(item),
					  "station_website",
					  (gpointer) station_website);
			gtk_widget_show(item);
			g_free(label);

			/* selection */
			if (selected_station_uri != NULL &&
			    !strcmp(selected_station_uri, station_uri))
				selection = i;
		} else {
			g_free(station_uri);
			g_free(station_name);
			g_free(station_location);
			g_free(station_band);
			g_free(station_description);
		}
		i++;
		stationdata = stationdata->next;
	}

	/* drop_down = gtk_combo_box_new(); */
	/* gtk_widget_show(drop_down); */
	/* gtk_combo_box_popdown(GTK_OPTION_MENU(drop_down), menu); */
	/* gtk_container_add(GTK_CONTAINER(align), drop_down); */

	if (selection != -1)
		gtk_combo_box_set_active(GTK_COMBO_BOX(drop_down), selection);

	g_signal_connect(G_OBJECT(stations_selector), "response",
			 G_CALLBACK(gtk_widget_hide),
			 (gpointer) stations_selector);
	g_signal_connect(G_OBJECT(stations_selector), "delete-event",
			 G_CALLBACK(gtk_widget_hide),
			 (gpointer) stations_selector);

	return stations_selector;
}

static gboolean
on_location_matches(GtkEntryCompletion *widget,
		    GtkTreeModel *model,
		    GtkTreeIter *iter,
		    gpointer user_data)
{
	GValue value = {0, };

	gtk_tree_model_get_value(model, iter, STATION_LOCATION, &value);
	gnome_radio->selected_station_location = g_strdup(g_value_get_string(&value));
	g_value_unset(&value);

	/* appbar_send_msg(_("Found location %s"), */
	/* 		gnome_radio->selected_station_location); */
	/* gnome_radio_helper_run(gnome_radio->selected_station_uri, */
	/* 		gnome_radio->selected_station_name, */
	/* 		GNOME_RADIO_STREAM_SHOUTCAST, */
	/* 		GNOME_RADIO_STREAM_PLAYER); */
	return FALSE;
}

void
save_cb (GtkWidget *widget, gpointer data) {

	/* g_print("%s\n", data); */
        gint context_id;
        char *nameentry, *locationentry, *urientry, *websiteentry, *descriptionentry, *statusmsg;
	nameentry = g_object_get_data(G_OBJECT(widget), "station_name");
	locationentry = g_object_get_data(G_OBJECT(widget), "station_location");
	urientry = g_object_get_data(G_OBJECT(widget), "station_uri");
	websiteentry = g_object_get_data(G_OBJECT(widget), "station_website");
	descriptionentry = g_object_get_data(G_OBJECT(widget), "station_description");
	context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "Station Name");
	/* Translators: first %s is station name and second %s is station location */
	statusmsg = g_strdup_printf(_("Added %s in %s"), nameentry, locationentry);
	gtk_statusbar_push (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id), statusmsg);
	g_free(statusmsg);
	GNOME_RADIO_DEBUG_MSG("%s\n", nameentry);
	GNOME_RADIO_DEBUG_MSG("%s\n", locationentry);
	GNOME_RADIO_DEBUG_MSG("%s\n", urientry);
	GNOME_RADIO_DEBUG_MSG("%s\n", websiteentry);
	GNOME_RADIO_DEBUG_MSG("%s\n", descriptionentry);
}

GtkWidget *create_new_station_selector(gchar *location) {

	GtkWidget *station_selector, *content_area;
	GtkWidget *align;
	GtkWidget *bandentry, *descriptionentry, *nameentry, *locationentry, *urientry, *websiteentry;
	GtkEntryCompletion *completion;
	GtkListStore *location_model;
	GtkTreeIter iter;
	TzDB *db;
	GPtrArray *locs;
	guint i;
	char *pixmap_dir = NULL;
	gchar *path = NULL;
	GtkWidget *item = NULL;
	setlocale (LC_ALL, "C");

	gchar *world_station_xml_filename, *local_station_xml_file;
	gint retval;
	
	/* int i = 0, search_selection = -1; */

	GStatBuf stats;

	memset(&stats, 0, sizeof(stats));

	/* The Stations dialog */
	station_selector = gtk_dialog_new_with_buttons(_("New Internet Radio Station"),
						       GTK_WINDOW(gnome_radio_app),
						       0,
						       (_("_Save")),
						       GTK_RESPONSE_ACCEPT,
						       NULL);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (station_selector));

	g_signal_connect(G_OBJECT(station_selector), "response", G_CALLBACK(save_cb), G_OBJECT(station_selector));
	/* gtk_container_set_border_width */
	/* 	(GTK_CONTAINER(GTK_DIALOG(station_selector)->vbox), 6);  */

	/* align = gtk_alignment_new(0.5, 0.5, 0, 0);  */
	/* gtk_container_add(GTK_CONTAINER */
	/* 		  (GTK_DIALOG(station_selector)->vbox), align);  */
	/* gtk_container_set_border_width(GTK_CONTAINER(align), 6); */
	/* gtk_widget_show(align); */

	bandentry = gtk_entry_new();
	nameentry = gtk_entry_new();
	locationentry = gtk_entry_new();
	urientry = gtk_entry_new();
	websiteentry = gtk_entry_new();
	descriptionentry = gtk_entry_new();

	gtk_entry_set_text(GTK_ENTRY(nameentry), _("Station name"));
	gtk_entry_set_text(GTK_ENTRY(bandentry), _("Bandwidth"));
	if (!g_strcmp0(gtk_entry_get_text(GTK_ENTRY(input)),"")) {
		gtk_entry_set_text(GTK_ENTRY(locationentry), _("City name"));
	} else {
		gtk_entry_set_text(GTK_ENTRY(locationentry), (gpointer)gtk_entry_get_text(GTK_ENTRY(input)));
	}
	gtk_entry_set_text(GTK_ENTRY(urientry), _("http://uri-to-stream/"));
	gtk_entry_set_icon_from_stock(GTK_ENTRY(urientry), GTK_ENTRY_ICON_PRIMARY, GTK_STOCK_CONNECT);
	gtk_entry_set_icon_tooltip_text(GTK_ENTRY(locationentry), GTK_ENTRY_ICON_SECONDARY, "Type the stream URL (http://) you want to listen to radio from");
	gtk_entry_set_text(GTK_ENTRY(descriptionentry), _("Description"));
	gtk_entry_set_icon_from_stock(GTK_ENTRY(websiteentry), GTK_ENTRY_ICON_PRIMARY, GTK_STOCK_CONNECT);
	gtk_entry_set_icon_tooltip_text(GTK_ENTRY(websiteentry), GTK_ENTRY_ICON_SECONDARY, "Type the home page URL (http://) you want to connect with the station");
	gtk_entry_set_text(GTK_ENTRY(websiteentry), _("http://uri-to-website/"));
	completion = gtk_entry_completion_new();
	gtk_entry_completion_set_text_column(completion, STATION_LOCATION);
	gtk_entry_set_completion(GTK_ENTRY(locationentry), completion);
	gtk_entry_set_icon_from_stock(GTK_ENTRY(locationentry), GTK_ENTRY_ICON_PRIMARY, GTK_STOCK_JUMP_TO);
	gtk_entry_set_icon_tooltip_text(GTK_ENTRY(locationentry), GTK_ENTRY_ICON_SECONDARY, "Type the city name you want to listen to radio around");
	g_signal_connect(G_OBJECT(completion), "match-selected",
			 G_CALLBACK(on_location_matches), NULL);
	location_model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	world_station_xml_filename = g_strconcat(GNOME_RADIO_DATADIR, "/gnome-radio.xml", NULL);
	GNOME_RADIO_DEBUG_MSG("world_station_xml_filename = %s\n",
	    world_station_xml_filename);
	if (world_station_xml_filename == NULL) {
		g_warning((_("Failed to open %s.  Please install it.\n")),
			  world_station_xml_filename);
	}
	local_station_xml_file =
	    g_strconcat(g_get_home_dir(), "/.gnome_radio/gnome_radio.xml", NULL);
	if (!g_stat(local_station_xml_file, &stats)) {
		localstation = gnome_radio_station_load_from_file(NULL, local_station_xml_file);
	} else {
		localstation = NULL;
	}
	if (localstation == NULL) {
		g_warning(_("Failed to open %s\n"), local_station_xml_file);
	}
	stationdata = gnome_radio_station_load_from_file(localstation,
									  world_station_xml_filename);
	gtk_container_add(GTK_CONTAINER(content_area), nameentry);
	gtk_container_add(GTK_CONTAINER(content_area), bandentry);
	gtk_container_add(GTK_CONTAINER(content_area), locationentry);
	gtk_container_add(GTK_CONTAINER(content_area), urientry);
	gtk_container_add(GTK_CONTAINER(content_area), descriptionentry);
	gtk_container_add(GTK_CONTAINER(content_area), websiteentry);
	gtk_widget_show(nameentry);
	gtk_widget_show(bandentry);
	gtk_widget_show(locationentry);
	gtk_widget_show(urientry);
	gtk_widget_show(descriptionentry);
	gtk_widget_show(websiteentry);
	/* g_signal_connect(G_OBJECT(station_selector), GTK_RESPONSE_ACCEPT, */
	/* 		 G_CALLBACK(on_new_station_selector_changed), */
	/* 		 NULL); */
	g_object_set_data(G_OBJECT(station_selector), "station_name",
			  (gchar *) gtk_entry_get_text(GTK_ENTRY(nameentry)));
	g_object_set_data(G_OBJECT(station_selector), "station_band",
			  (gchar *) gtk_entry_get_text(GTK_ENTRY(bandentry)));
	g_object_set_data(G_OBJECT(station_selector), "station_location",
			  (gchar *) gtk_entry_get_text(GTK_ENTRY(locationentry)));
	g_object_set_data(G_OBJECT(station_selector), "station_uri",
			  (gchar *) gtk_entry_get_text(GTK_ENTRY(urientry)));
	g_object_set_data(G_OBJECT(station_selector), "station_description",
			  (gchar *) gtk_entry_get_text(GTK_ENTRY(descriptionentry)));
	g_object_set_data(G_OBJECT(station_selector), "station_website",
			  (gchar *) gtk_entry_get_text(GTK_ENTRY(websiteentry)));
	GNOME_RADIO_DEBUG_MSG("WEBSITEENTRY: %s\n", (gchar *) gtk_entry_get_text(GTK_ENTRY(websiteentry)));
#if 0 /* FIXME: Add input fields */
	g_object_set_data(G_OBJECT(station_selector), "station_description",
			  (gchar *) station_description);
	g_object_set_data(G_OBJECT(station_selector), "station_website",
			  (gchar *) station_website);
#endif
	// gtk_widget_show(station_selector);
	// g_free(label);
	g_signal_connect(G_OBJECT(station_selector), "response",
			 G_CALLBACK(gtk_widget_hide),
			 (gpointer) station_selector);
	g_signal_connect(G_OBJECT(station_selector), "delete-event",
			 G_CALLBACK(gtk_widget_hide),
			 (gpointer) station_selector);
	/* tz_db_free (db); */
	/* g_free (pixmap_dir); */
	/* g_free (filename); */
	/* g_free (path); */
	return station_selector;
}

GtkWidget *create_gnome_radio_app(void)
{
	GtkWidget *gnome_radio_app;
	GtkWidget *vbox1;
	GtkWidget *gnome_radio_pixmap;
	GtkWidget *appbar;
	GtkWidget *progress;
	gsize length;
	const gchar *selected_station, *selected_station_uri, *selected_station_name, *selected_station_location, *selected_station_description;
	GNOMERadioInfo *gnome_radio_data = g_new0(GNOMERadioInfo, 1);
	char *pmf;
	gtk_window_set_title(GTK_WINDOW(gnome_radio_app), "GNOME Radio 13");
	gnome_radio = gnome_radio_data;
	/* gnome_radio_data->settings = g_settings_new(GNOME_RADIO_UI); */
	/* selected_station_uri = g_variant_get_string(g_settings_get_value (gnome_radio_data->settings, "selected_station_uri"), &length); */
	/* selected_station = g_variant_get_string(g_settings_get_value(gnome_radio_data->settings, "station"), &length); */
	GNOME_RADIO_DEBUG_MSG ("SELECTED STATION = %s\n", selected_station);
	/* gnome_radio->selected_station_uri = selected_station_uri; */
	selected_station_name = g_variant_get_string(g_settings_get_value (gnome_radio_data->settings, "selected_station_name"), &length);
	/* gnome_radio->selected_station_name = selected_station_name; */
	selected_station_location = g_variant_get_string(g_settings_get_value (gnome_radio_data->settings, "selected_station_location"), &length);
	/* gnome_radio->selected_station_location = selected_station_location; */
	selected_station_description = g_variant_get_string(g_settings_get_value (gnome_radio_data->settings, "selected_station_description"), &length);
	/* gnome_radio->selected_station_description = selected_station_description; */
#if GNOME_RADIO_CFG_GNOME_CONFIG
	gnome_config_push_prefix("/gnome-radio/General/");
	gnome_radio->selected_listener_uri =
	    gnome_config_get_string("selected_listener_uri=");
	gnome_radio->selected_listener_name =
	    gnome_config_get_string("selected_listener_name=");
	gnome_radio->selected_listener_location =
	    gnome_config_get_string("selected_listener_location=");
	gnome_radio->selected_listener_description =
	    gnome_config_get_string("selected_listener_description=");
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_listener_uri: %s\n",
	       gnome_radio->selected_listener_uri);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_listener_name: %s\n",
	       gnome_radio->selected_listener_name);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_listener_location: %s\n",
	       gnome_radio->selected_listener_location);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_listener_band: %s\n",
	       gnome_radio->selected_listener_band);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_listener_description: %s\n",
	       gnome_radio->selected_listener_description);
	gnome_radio->selected_station_uri =
	    gnome_config_get_string("selected_station_uri=");
	gnome_radio->selected_station_name =
	    gnome_config_get_string("selected_station_name=");
	gnome_radio->selected_station_location =
	    gnome_config_get_string("selected_station_location=");
	gnome_radio->selected_station_description =
	    gnome_config_get_string("selected_station_description=");
	gnome_radio->selected_station_name =
	    gnome_config_get_string("selected_station_name=");
	gnome_radio->selected_station_location =
	    gnome_config_get_string("selected_station_location=");
	gnome_radio->selected_station_description =
	    gnome_config_get_string("selected_station_description=");
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_station_uri: %s\n",
	       gnome_radio->selected_station_uri);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_station_name: %s\n",
	       gnome_radio->selected_station_name);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_station_location: %s\n",
	       gnome_radio->selected_station_location);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_station_description: %s\n",
	       gnome_radio->selected_station_description);
	if (strcmp(gnome_radio->selected_station_uri,"")==0) {
		gnome_radio->selected_station_uri = g_strdup(_("http://fm939.wnyc.org/wnycfm"));
	}
	if (strcmp(gnome_radio->selected_station_name,"")==0) {
		gnome_radio->selected_station_name = g_strdup(_("WNYC"));
	}
	if (strcmp(gnome_radio->selected_station_location,"")==0) {
		gnome_radio->selected_station_location = g_strdup(_("New York City, NY"));
	}
	if (strcmp(gnome_radio->selected_station_band,"")==0) {
		gnome_radio->selected_station_band = g_strdup(_("Online"));
	}
	if (strcmp(gnome_radio->selected_station_description,"")==0) {
		gnome_radio->selected_station_description = g_strdup(_("WNYC 93.9 FM and AM 820 are New York's flagship public radio stations, broadcasting the finest programs from NPR, American Public Media, Public Radio International and the BBC World Service, as well as a wide range of award-winning local programming."));
	}
	gnome_radio->selected_streams_uri =
		gnome_config_get_string("selected_streams_uri=");
	gnome_radio->selected_streams_mime =
	    gnome_config_get_string("selected_streams_mime=");
	gnome_radio->selected_streams_codec =
	    gnome_config_get_string("selected_streams_codec=");
	gnome_radio->selected_streams_bitrate =
	    gnome_config_get_string("selected_streams_bitrate=");
	gnome_radio->selected_streams_samplerate =
	    gnome_config_get_string("selected_streams_samplerate=");
	gnome_radio->selected_streams_channels =
		(Gnome_Internet_Radio_LocatorChannels)gnome_config_get_string("selected_streams_channels=");
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_streams_uri: %s\n",
	       gnome_radio->selected_streams_uri);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_streams_mime: %s\n",
	       gnome_radio->selected_streams_mime);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_streams_codec: %s\n",
	       gnome_radio->selected_streams_codec);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_streams_bitrate: %s\n",
	       gnome_radio->selected_streams_bitrate);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_streams_samplerate: %s\n",
	       gnome_radio->selected_streams_samplerate);
	GNOME_RADIO_DEBUG_MSG("gnome_radio->selected_channels: %0x\n",
	       gnome_radio->selected_streams_channels);
	gnome_config_pop_prefix();
#endif
	return gnome_radio_app;
}
