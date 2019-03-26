#include <gtk/gtk.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha256.h"
#include "interface_abstraction_layer.h"
#include "run_length_encoding.h"
#include "lempel_ziv_welch.h"

#define TEST 1

GtkWidget* g_main_window;
GtkWidget* g_laba2_window;
GtkWidget* g_laba3_window;
GtkWidget* entry;
const char* g_choicer_path = "/home/bies/Dev/Programming/C/InformationSecurity/Laba2";
		
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

void Password_Push_Button_Clicked(
		GtkWidget* button,
        gpointer pointer)
{
	gchar* entry_text = gtk_entry_get_text(entry);
	char hash[32];
	SHA256_CTX ctx;
	char* final_key_enc = (char*) malloc(4*32*sizeof(char));
	
	for (int i = 0; i < 4; i++)
	{
		sha256_init(&ctx);
		sha256_update(&ctx, entry_text, strlen(entry_text));
		sha256_final(&ctx, (int8_t*) hash);
		final_key_enc = mstring_concat(final_key_enc, hash);
	}
	char* path = mstring_concat(g_choicer_path, "/key.enc");
	mio_write_binary_file(path, "wb", final_key_enc, strlen(final_key_enc));
	printf("Password_Button_Clicked: path = %s\nPassword_Button_Clicked: hash = %s\nentry_text = %s\n", 
		path, final_key_enc, entry_text);
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
void Folder_Changed_Laba1(GtkFileChooser* fChooser1)
{
	char* folder_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fChooser1));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fChooser1), folder_name);	
	folder_changed_laba1_event(folder_name);
}
//Обработчик события изменения файла
void Folder_Changed_Laba3(GtkFileChooser* fChooser1)
{
	char* folder_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fChooser1));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fChooser1), folder_name);	
	folder_changed_laba3_event(folder_name);
}

//Создание диалога (всплывающее окно с сообщением)
void CreateDialog(const char* message, const char* title)
{
	GtkWidget *evd_dialog = gtk_message_dialog_new(g_laba2_window, GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
	gtk_window_set_title(GTK_WINDOW(evd_dialog), title);	
	gtk_dialog_run(GTK_DIALOG(evd_dialog));
	gtk_widget_destroy(evd_dialog);
}

void Laba1GUI(int argc, char** argv)
{
	GtkWidget *enc_button,
    		  *dec_button,
    		  *save_button,
    		  *exit_button,
    		  *chooser1,
    		  *label;
    GtkFileFilter *filter1;
    GtkWidget *vertical_box;    
    
    gtk_init(&argc, &argv);
	
    g_laba2_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(g_laba2_window), "Шифрование"); 

    g_signal_connect(
        g_laba2_window, "destroy",
        G_CALLBACK(WindowDestroyEvent), 0);
    g_signal_connect(
        g_laba2_window, "delete-event",
        G_CALLBACK(WindowDeleteEvent), 0);

    gtk_container_set_border_width(
        GTK_CONTAINER(g_laba2_window), 65);
    gtk_widget_show(g_laba2_window);
    
    //Label
	label = gtk_label_new("");
	gtk_widget_show(label);
	
	//File searching
    chooser1 = gtk_file_chooser_button_new(
		"Chooser a Folder",
    	GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_widget_show(chooser1);	
    	
    g_signal_connect(chooser1, 
		"selection_changed",
    	G_CALLBACK(Folder_Changed_Laba1), 0);	
    
    //g_get_home_dir()
    const char* choicer_path = 
		"/home/bies/Dev/Programming/C/InformationSecurity/Laba2";
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(chooser1), 
		choicer_path);
    
	//filter1
    filter1 = gtk_file_filter_new();
    gtk_file_filter_set_name(filter1, "All Files");
    gtk_file_filter_add_pattern(filter1, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(chooser1), filter1);
    
    //Buttons    
    enc_button = gtk_button_new_with_label("Encrypt data");
    g_signal_connect(
        enc_button, "clicked",
        G_CALLBACK(Encrypt_Clicked), 0);
    gtk_widget_show(enc_button);
	
	dec_button = gtk_button_new_with_label("Decrypt data");
    g_signal_connect(
        dec_button, "clicked",
        G_CALLBACK(Decrypt_Clicked), 0);
    gtk_widget_show(dec_button);
	
	save_button = gtk_button_new_with_label("Save");
    g_signal_connect(
        save_button, "clicked",
        G_CALLBACK(Save_Clicked), 0);
    gtk_widget_show(save_button);
	
	exit_button = gtk_button_new_with_label("Exit");
    g_signal_connect(
        exit_button, "clicked",
        G_CALLBACK(WindowDestroyEvent), 0);
    gtk_widget_show(exit_button);
	
    vertical_box = gtk_box_new(TRUE, 10);
    gtk_widget_show(vertical_box);
    AddVBox(vertical_box, chooser1, FALSE, TRUE, 0);
    AddVBox(vertical_box, enc_button, FALSE, TRUE, 0);
    AddVBox(vertical_box, dec_button, FALSE, TRUE, 0);
    AddVBox(vertical_box, save_button, FALSE, TRUE, 0);	
    AddVBox(vertical_box, exit_button, FALSE, TRUE, 0);	
		
    gtk_container_add(GTK_CONTAINER(g_laba2_window), vertical_box);
 	   
    gtk_main();
} 

void Laba3GUI(int argc, char** argv)
{
	GtkWidget *enc_button,
    		  *dec_button,
    		  *save_button,
    		  *exit_button,
    		  *chooser1,
			  *password_button;
    GtkFileFilter *filter1;
    GtkWidget *vertical_box;    
    
    gtk_init(&argc, &argv);
	
    g_laba2_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(g_laba2_window), "Шифрование"); 

    g_signal_connect(
        g_laba2_window, "destroy",
        G_CALLBACK(WindowDestroyEvent), 0);
    g_signal_connect(
        g_laba2_window, "delete-event",
        G_CALLBACK(WindowDeleteEvent), 0);

    gtk_container_set_border_width(
        GTK_CONTAINER(g_laba2_window), 65);
    gtk_widget_show(g_laba2_window);
    
	
	//File searching
    chooser1 = gtk_file_chooser_button_new(
		"Chooser a Folder",
    	GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_widget_show(chooser1);
    g_signal_connect(chooser1, 
		"selection_changed",
    	G_CALLBACK(Folder_Changed_Laba3), 0);
    gtk_file_chooser_set_current_folder(
		GTK_FILE_CHOOSER(chooser1), 
		g_choicer_path);
    
	//filter1
    filter1 = gtk_file_filter_new();
    gtk_file_filter_set_name(filter1, "All Files");
    gtk_file_filter_add_pattern(filter1, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(chooser1), filter1);
    
    //Entry
	entry = gtk_entry_new();
	gtk_widget_show(entry);
	
	password_button = gtk_button_new_with_label("Push password");
    g_signal_connect(
        password_button, "clicked",
        G_CALLBACK(Password_Push_Button_Clicked), 0);
    gtk_widget_show(password_button);
	
    //Buttons    
    enc_button = gtk_button_new_with_label("Encrypt data");
    g_signal_connect(
        enc_button, "clicked",
        G_CALLBACK(Encrypt_Clicked), 0);
    gtk_widget_show(enc_button);
	
	dec_button = gtk_button_new_with_label("Decrypt data");
    g_signal_connect(
        dec_button, "clicked",
        G_CALLBACK(Decrypt_Clicked), 0);
    gtk_widget_show(dec_button);
	
	save_button = gtk_button_new_with_label("Save");
    g_signal_connect(
        save_button, "clicked",
        G_CALLBACK(Save_Clicked), 0);
    gtk_widget_show(save_button);
	
	exit_button = gtk_button_new_with_label("Exit");
    g_signal_connect(
        exit_button, "clicked",
        G_CALLBACK(WindowDestroyEvent), 0);
    gtk_widget_show(exit_button);
	
    vertical_box = gtk_box_new(TRUE, 10);
    gtk_widget_show(vertical_box);
    AddVBox(vertical_box, chooser1, FALSE, TRUE, 0);
    AddVBox(vertical_box, entry, FALSE, TRUE, 0);
    AddVBox(vertical_box, password_button, FALSE, TRUE, 0);
    AddVBox(vertical_box, enc_button, FALSE, TRUE, 0);
    AddVBox(vertical_box, dec_button, FALSE, TRUE, 0);
    AddVBox(vertical_box, save_button, FALSE, TRUE, 0);	
    AddVBox(vertical_box, exit_button, FALSE, TRUE, 0);	
		
    gtk_container_add(GTK_CONTAINER(g_laba2_window), vertical_box);
 	   
    gtk_main();
} 

void Laba4_GUI(int argc, char** argv)
{
	
}

//entry point
int main(int argc,
         char** argv)
{
    g_print("__________Tea encryption/decryption algorithm__________\n");  
	
#if TEST == 1
	
	#if 0
	{
		char char4[4] = "azzd";
		char char8[8] = "abcdzera";
		int32_t cnum32 = convert_char4_to_int32(char4);
		int64_t cnum64 = convert_char8_to_int64(char8);
		char* char_4 = convert_int32_to_char4(cnum32);
		char* char_8 = convert_int64_to_char8(cnum64);
		int64_t i64 = cnum32;
		i64 = add_high_bytes_to_int64(i64, cnum32);
		char* conver = convert_int64_to_char8(i64);
		
		printf("cnum32 = %d\n", cnum32);
		printf("cnum64 = %lld\n", cnum64);
		printf("char4 = %s\n", char_4);
		printf("char8 = %s\n", char_8);
		printf("i64_to_string = %s\n", conver);
		
		int32_t cnum32_2 = get_low_bytes_from_int64(cnum64);
		char_4 = convert_int32_to_char4(cnum32_2);
		printf("low_bytes = %s\n", char_4);
		
		cnum32_2 = get_high_bytes_from_int64(cnum64);
		char_4 = convert_int32_to_char4(cnum32_2);
		printf("high_bytes = %s\n", char_4);
		
		int32_t ivec0 = 0x11234562;
		int32_t v0 = 123;		
		int32_t val = (v0 | (ivec0 & (0xff>>8)));
		printf("val = %d\n", val);
		
		sha256_test();
	}
	#else
	
	//RLE
	//data_compression();
	
	//LZW
	lzw_test();
	
	/*
	char* temp = mstring_concat_chars('a', 'b');
	printf("temp = %s\n", temp);
	printf("bool = %d\n", (strcmp(temp, "ab") == 0));
	
	int32_t x = 0;
	x = set32(x, 0);
	x = set32(x, 1);
	x = set32(x, 2);
	x = set32(x, 3);
	
	int32_t y = 0;
	y = set32(y, 2);
	y = set32(y, 4);
	
	int32_t xy = x ^ y;
	print_int32_to_binary(xy);
	*/
	
	
	#endif
	
#elif TEST == 0
	
	//Laba1GUI(argc, argv);
    Laba3GUI(argc, argv);
	
#endif
	
	DestroyData();
	
    return(0);
}


/*
NOTE (done work):
 * - 1
 * - 2
TODO(3):
 * - функция проверки на наличие подстроки
 * - окно для введения пароля
 * - 
 * */
 
 
 /*
	key =  Alloc(4);
	int length;
	int isKeyFileExist = 0;
	char* keyStr = ReadFileSafe("key.txt", &isKeyFileExist, &length);
	//WriteFile("new_file_name_file_name.txt", "info", 4);
	if (isKeyFileExist == 1)
	{
		if (keyStr)
		{
			int index; 
			char* temp = strtok(keyStr, "\n");
			key[0] = atoi(temp);
			for (int i = 1 ; ; i++) 
			{
				temp = strtok(NULL, "\n");
				if (temp == NULL) break;
				key[i] = atoi(temp);
			}
		}
	}
	*/