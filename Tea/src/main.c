#include <gtk/gtk.h>

#include <stdint.h>
#include <stdio.h>
#include "string.h"

#include "additional.h"

//#define TEST

GtkWidget *evd_window;

//Обработчик события уничтожения окна
void WindowDestroyEvent(
        GtkWidget* window,
        gpointer pointer)
{
    g_print("Destoying window\n");
    gtk_main_quit();
}

//Обработчик события удаления окна
gboolean WindowDeleteEvent(
    GtkWidget* widget,
    GdkEvent* event,
    gpointer pointer)
{
    g_print("Window delete event\n");
    return(FALSE);
}	
	
//Обработчик события нажатия на EncryptButton
void Encrypt_Clicked(
        GtkWidget* button,
        gpointer pointer)
{
	Encrypt();
}

//Обработчик события нажатия на DecryptButton
void Decrypt_Clicked(
        GtkWidget* button,
        gpointer pointer)
{
	Decrypt();
}

//Обработчик события нажатия на SaveButton
void Save_Clicked(
	GtkWidget* button,
    gpointer pointer)
{
	SaveFile();
	CreateDialog("File saved!", "Event");
	g_print("File saved!\n");
}

//Упрощение написания функции добавления Widget* в VBox
void AddVBox(GtkWidget *vBox, GtkWidget *widget, int val1, int val2, int val3)
{
    gtk_box_pack_start(GTK_BOX(vBox), widget, val1, val2, val3);
}

//Обработчик события изменения файла
void Folder_Changed(GtkFileChooser* fChooser1)
{
	char* evd_FolderName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fChooser1));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fChooser1), evd_FolderName);	
	FolderChanged(evd_FolderName);
}

//Создание диалога (всплывающее окно с сообщением)
void CreateDialog(const char* message, const char* title)
{
	GtkWidget *evd_dialog = gtk_message_dialog_new(evd_window, GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
	gtk_window_set_title(GTK_WINDOW(evd_dialog), title);	
	gtk_dialog_run(GTK_DIALOG(evd_dialog));
	gtk_widget_destroy(evd_dialog);
}

//entry point
int main(int argc,
         char** argv)
{
    g_print("__________Tea encryption/decryption algorithm__________\n");  
	
	DataInit();
	
#ifdef TEST
	
	entry();
	
#else
	
    GtkWidget *evd_encButton,
    		  *evd_decButton,
    		  *evd_saveButton,
    		  *evd_exitButton,
    		  *evd_chooser1,
    		  *evd_label;
    GtkFileFilter *evd_filter1, 
    		      *evd_filter2;
    GtkWidget *evd_vBox;    
    
    gtk_init(&argc, &argv);    
    
    evd_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(evd_window), "Заголовок");

    g_signal_connect(
        evd_window, "destroy",
        G_CALLBACK(WindowDestroyEvent), 0);
    g_signal_connect(
        evd_window, "delete-event",
        G_CALLBACK(WindowDeleteEvent), 0);

    gtk_container_set_border_width(
        GTK_CONTAINER(evd_window), 50);
    gtk_widget_show(evd_window);
    
    //Label
	evd_label = gtk_label_new("");
	gtk_widget_show(evd_label);
	
	//File searching
    evd_chooser1 = gtk_file_chooser_button_new("Chooser a Folder",
    	GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_widget_show(evd_chooser1);	
    	
    g_signal_connect(evd_chooser1, "selection_changed",
    	G_CALLBACK(Folder_Changed), 0);	
    
    //g_get_home_dir()
    const char* evd_pathChoicer = 
		"/home/bies/Dev/Programming/C/InformationSecurity/Laba1";
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(evd_chooser1), 
		evd_pathChoicer);
    
    evd_filter1 = gtk_file_filter_new();
    gtk_file_filter_set_name(evd_filter1, "All Files");
    gtk_file_filter_add_pattern(evd_filter1, "*");
	
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(evd_chooser1), evd_filter1);
    
    //Buttons    
    evd_encButton = gtk_button_new_with_label("Encrypt data");
    g_signal_connect(
        evd_encButton, "clicked",
        G_CALLBACK(Encrypt_Clicked), 0);
    gtk_widget_show(evd_encButton);
	
	evd_decButton = gtk_button_new_with_label("Decrypt data");
    g_signal_connect(
        evd_decButton, "clicked",
        G_CALLBACK(Decrypt_Clicked), 0);
    gtk_widget_show(evd_decButton);
	
	evd_saveButton = gtk_button_new_with_label("Save");
    g_signal_connect(
        evd_saveButton, "clicked",
        G_CALLBACK(Save_Clicked), 0);
    gtk_widget_show(evd_saveButton);
	
	evd_exitButton = gtk_button_new_with_label("Exit");
    g_signal_connect(
        evd_exitButton, "clicked",
        G_CALLBACK(WindowDestroyEvent), 0);
    gtk_widget_show(evd_exitButton);
	
    evd_vBox = gtk_box_new(TRUE, 10);
    gtk_widget_show(evd_vBox);
    AddVBox(evd_vBox, evd_chooser1, FALSE, TRUE, 0);
    AddVBox(evd_vBox, evd_encButton, FALSE, TRUE, 0);
    AddVBox(evd_vBox, evd_decButton, FALSE, TRUE, 0);
    AddVBox(evd_vBox, evd_saveButton, FALSE, TRUE, 0);	
    AddVBox(evd_vBox, evd_exitButton, FALSE, TRUE, 0);	
		
    gtk_container_add(GTK_CONTAINER(evd_window), evd_vBox);
 	   
    gtk_main();
	
#endif
	
	DestroyData();
	
    return(0);
}

/*
TODO:
	char** parStr;
	char* we = "string";
	long we_len = strlen(we);
	g_print("we.len = %ld\n", we_len);
	
	parStr[0] = "0_el"; //GetNByte(str, 8, 0);
	//TODO: fix bug with "string" - 6 bytes
	//interpreting like "stringin" - 8 bytes 
	//(what's the problem?! может внутренности компилятора работают неверно)
	//parStr[1] = "000_00";
	memcpy(
		(void*)parStr[1], 
		we, 
		we_len
		);
	g_print("parStr[1].length = %ld\n", strlen(parStr[1]));
	parStr[2] = "is";
	parStr[3] = "i";
	parStr[4] = "s";
	
	for(int i = 0; i < nblocks-1; i++)
	{
		//error
		g_print("parStr[%d] = %s\n", i, parStr[i]);
		//parStr[i] = GetNByte(longString, 1, i*);
		//parStr[i] = AlloC(8);	
	}

*/
