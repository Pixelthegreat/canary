#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

/* forward declarations */
void destroy(GtkWidget *, gpointer);
gboolean closeweb(WebKitWebView *, GtkWidget *);
gboolean loadweb(GtkWidget *, GtkWidget *);
gboolean backweb(GtkWidget *, GtkWidget *);
gboolean forwweb(GtkWidget *, GtkWidget *);

WebKitWebView *web; /* web view */
WebKitWebContext *wctx; /* web context */
WebKitCookieManager *wckm; /* web cookie manager */
GtkWidget *search; /* search bar */

/* main(int argc, char *argv[]) */
int main(int argc, char *argv[]) {

	GtkWidget *win; /* the main window */
	GtkWidget *vbox; /* box */
	GtkWidget *htbbox; /* toolbar box */
	GtkWidget *gobtn; /* search button */
	GtkWidget *backbtn; /* to go back */
	GtkWidget *forwbtn; /* to go forward */

	/* gtk init */
	gtk_init(&argc, &argv);
	
	/* create window */
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 640, 480);

	/* set container */
	gtk_container_set_border_width(GTK_CONTAINER(win), 10);
	gtk_window_set_title(GTK_WINDOW(win), "canary");

	/* create box */
	vbox = gtk_box_new(1, 2);
	gtk_container_add(GTK_CONTAINER(win), vbox);
	
	/* create toolbar box */
	htbbox = gtk_box_new(0, 2);
	gtk_box_pack_start(GTK_BOX(vbox), htbbox, FALSE, FALSE, 1);

	/* create back and forward buttons */
	backbtn = gtk_button_new_with_label("<");
	gtk_box_pack_start(GTK_BOX(htbbox), backbtn, FALSE, FALSE, 1);
	
	forwbtn = gtk_button_new_with_label(">");
	gtk_box_pack_start(GTK_BOX(htbbox), forwbtn, FALSE, FALSE, 1);

	/* create search bar */
	search = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(htbbox), search, TRUE, TRUE, 1);

	/* create go button */
	gobtn = gtk_button_new_with_label("go");
	gtk_box_pack_start(GTK_BOX(htbbox), gobtn, FALSE, FALSE, 1);

	/* create web view */
	web = WEBKIT_WEB_VIEW(webkit_web_view_new());
	gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(web), TRUE, TRUE, 1);

	/* context and cookie management */
	wctx = webkit_web_context_get_default();
	wckm = webkit_web_context_get_cookie_manager(wctx);

	/* enable cookies */
	webkit_cookie_manager_set_persistent_storage(wckm, "cookies.txt",
				WEBKIT_COOKIE_PERSISTENT_STORAGE_TEXT);
	webkit_cookie_manager_set_accept_policy(wckm, WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS);

	/* set callbacks */
	g_signal_connect(win, "destroy", G_CALLBACK(destroy), NULL);
	g_signal_connect(web, "close", G_CALLBACK(closeweb), win);
	g_signal_connect(search, "activate", G_CALLBACK(loadweb), win);
	g_signal_connect(gobtn, "clicked", G_CALLBACK(loadweb), win);
	g_signal_connect(backbtn, "clicked", G_CALLBACK(backweb), win);
	g_signal_connect(forwbtn, "clicked", G_CALLBACK(forwweb), win);

	/* load a page */
	webkit_web_view_load_uri(web, "https://www.startpage.com/");
	
	/* show widgets */
	gtk_widget_grab_focus(GTK_WIDGET(vbox));
	gtk_widget_show_all(htbbox);
	gtk_widget_show_all(vbox);
	gtk_widget_show_all(win);
	
	/* main */
	gtk_main();
	
	return 0;
}

void destroy(GtkWidget *w, gpointer d) {
	gtk_main_quit();
}

gboolean closeweb(WebKitWebView *web, GtkWidget *win) {

	gtk_widget_destroy(win);
	return TRUE;
}

/* load url */
gboolean loadweb(GtkWidget *a, GtkWidget *b) {

	const char *url = gtk_entry_get_text(GTK_ENTRY(search));
	
	char *buf = (char *)malloc(strlen(url) + 9);
	memset(buf, 0, strlen(url) + 9);
	
	/* add https:// to beginning */
	if (!!strncmp(url, "https://", 8) && !!strncmp(url, "http://", 7) && !!strncmp(url, "file:///", 8)) {

		strcpy(buf, "https://");
	}
	
	/* add actual url */
	strcat(buf, url);
	
	printf("Loading '%s'...\n", buf);
	
	/* get webview to load url */
	webkit_web_view_load_uri(web, buf);
	free(buf);
	
	return TRUE;
}

/* go back */
gboolean backweb(GtkWidget *a, GtkWidget *b) {

	/* check if we can go back */
	if (webkit_web_view_can_go_back(web))
		webkit_web_view_go_back(web);
}

/* go forward */
gboolean forwweb(GtkWidget *a, GtkWidget *b) {

	/* check if we can go forward */
	if (webkit_web_view_can_go_forward(web))
		webkit_web_view_go_forward(web);
}
