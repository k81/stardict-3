#ifndef __SD_MAINWIN_H__
#define __SD_MAINWIN_H__

#include <gtk/gtk.h>
#include <string>

#include "articleview.h"
#include "readword.h"

const guint MAX_HISTORY_WORD_ITEM_NUM=20;
const guint MAX_BACK_WORD_ITEM_NUM=20;


const int MIN_WINDOW_WIDTH=200;
const int MIN_WINDOW_HEIGHT=100;

enum TextWinQueryResult
{
	TEXT_WIN_FOUND,
	TEXT_WIN_NOT_FOUND,
	TEXT_WIN_SHOW_FIRST,
	TEXT_WIN_FUZZY_NOT_FOUND,
	TEXT_WIN_PATTERN_NOT_FOUND,
	TEXT_WIN_TIPS, // when the word entry is empty,it will show tip
	TEXT_WIN_INFO,
	TEXT_WIN_TREEDICT,
	TEXT_WIN_NET_FOUND,
	TEXT_WIN_NET_NOT_FOUND,
	TEXT_WIN_NET_SHOW_FIRST,
};

enum TranslateEngineCode {
	TranslateEngine_Google,
	TranslateEngine_Yahoo,
	// TranslateEngine_Altavista,
	//TranslateEngine_SystranBox,
	TranslateEngine_ExciteJapan,
	TranslateEngine_Size // number of active engines. Must be the last item.
};


enum ListWinListWordType {
	LIST_WIN_NORMAL_LIST,
	LIST_WIN_FUZZY_LIST,
	LIST_WIN_DATA_LIST,
	LIST_WIN_PATTERN_LIST,
	LIST_WIN_EMPTY
};

struct BackListData {
	gchar *word;
	gdouble adjustment_value;
};

class TopWin {
private:
	gboolean enable_change_cb;
	GSList *BackList;
	GtkWidget *HistoryMenu;
	GtkWidget *BackMenu;
	GtkWidget* WordCombo;

	static gint HisCompareFunc(gconstpointer a,gconstpointer b);
	static gint BackListDataCompareFunc(gconstpointer a,gconstpointer b);

	static void ClearCallback(GtkWidget *widget, TopWin *oTopWin);
	static void GoCallback(GtkWidget *widget, TopWin *oTopWin);
	static void BackCallback(GtkWidget *widget, TopWin *oTopWin);
	static void MenuCallback(GtkWidget *widget, TopWin *oTopWin);
	static gboolean on_back_button_press(GtkWidget * widget, GdkEventButton * event , TopWin *oTopWin);
	static void on_back_menu_item_activate(GtkMenuItem *menuitem, gint index);

	static void on_entry_changed(GtkEntry *entry, TopWin *oTopWin);
	static void on_entry_activate(GtkEntry *entry, TopWin *oTopWin);
	static void on_entry_populate_popup(GtkEntry *entry, GtkMenu  *menu, TopWin *oTopWin);

	static void on_clear_history_menu_item_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	
	static void on_main_menu_preferences_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	static void on_main_menu_dictmanage_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	static void on_main_menu_pluginmanage_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	static void on_main_menu_newversion_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	static void on_main_menu_help_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	static void on_main_menu_about_activate(GtkMenuItem *menuitem, TopWin *oTopWin);
	static void on_main_menu_quit_activate(GtkMenuItem *menuitem, TopWin *oTopWin);

	void LoadHistory(GtkListStore* list_store);
	void SaveHistory(void);
public:
	GtkWidget* MainMenu;

	TopWin();
	~TopWin();

	void Create(GtkWidget *vbox);
	void Destroy(void);
	void SetText(const gchar *word, bool notify=true);
	//void SetText_without_notify(const gchar *word);
	void TextSelectAll();	
	void InsertHisList(const gchar *word);
	void InsertBackList(const gchar *word = NULL);
	void do_back();
	void do_prev();
	void do_next();
	void do_menu();

	gboolean TextSelected();
	bool has_focus() {
		return GTK_WIDGET_HAS_FOCUS(GTK_BIN(WordCombo)->child);
	}
	static void ClipboardReceivedCallback(GtkClipboard *clipboard, const gchar *text, gpointer data);

	void set_position_in_text(gint pos) {
		gtk_editable_set_position(GTK_EDITABLE(GTK_BIN(WordCombo)->child), pos);
	}
	void select_region_in_text(gint beg, gint end) {
		gtk_editable_select_region(GTK_EDITABLE(GTK_BIN(WordCombo)->child), beg, end);
	}
	const gchar *get_text() {
		if (!WordCombo)
			return "";
		return gtk_entry_get_text(GTK_ENTRY(GTK_BIN(WordCombo)->child));
	}
	void grab_focus() {
		gtk_widget_grab_focus(GTK_BIN(WordCombo)->child);
	}
};

class ListWin {
private:
	GtkListStore *list_model;
	GtkTreeStore *tree_model;
	bool nowIsList;

	static gboolean on_button_press(GtkWidget * widget, GdkEventButton * event, ListWin *oListWin);
	static void on_selection_changed(GtkTreeSelection *selection, ListWin *oListWin);

public:
	ListWinListWordType list_word_type;
	std::string fuzzyWord;
	GtkTreeView *treeview_;

	ListWin() {}
	void Create(GtkWidget *notebook);
	void SetModel(bool isListModel);
	void Destroy();
	void Clear();
	void ReScroll();
	void Prepend(const gchar *word);
	void InsertLast(const gchar *word);
	void SetTreeModel(std::vector<gchar *> *reslist, std::vector<InstantDictIndex> &dictmask);
	void SetTreeModel(std::list<STARDICT::LookupResponse::WordTreeElement *> *wordtree);
	void MergeFuzzyList(std::list<char *> *wordlist);
	void MergeWordList(std::list<char *> *wordlist);
	bool treeview_has_focus() const {
		return GTK_WIDGET_HAS_FOCUS(GTK_WIDGET(treeview_));
	}
};

class TreeWin {
public:
	GtkWidget *treeview;

	TreeWin() {}
	bool Create(GtkWidget *notebook);
private:
	static gboolean on_button_press(GtkWidget * widget, GdkEventButton * event, TreeWin *oTreeWin);
	static void on_selection_changed(GtkTreeSelection *selection, TreeWin *oTreeWin);
};

class ResultWin {
public:
	GtkWidget *treeview;
	void Create(GtkWidget *notebook);
	void InsertLast(const gchar *word, const gchar *mark);
	void Clear();
private:
	static void on_selection_changed(GtkTreeSelection *selection, ResultWin *oResultWin);
};

class IndexWin {
public:
	GtkWidget *notebook;

	ListWin oListWin;
	ResultWin oResultWin;
	TreeWin oTreeWin;

	IndexWin();
	bool Create(GtkWidget *hpaned);
private:
};

class LeftWin {
public:
	GtkWidget *vbox;
	LeftWin();
	~LeftWin();
	void Create(GtkWidget *hbox, bool has_treedict);
	void UpdateChooseGroup();
private:
	GtkWidget *choosegroup_menu;
	static void on_wazard_button_toggled(GtkToggleButton *button, LeftWin *oLeftWin);
	static void on_appendix_button_toggled(GtkToggleButton *button, LeftWin *oLeftWin);
	static void on_result_button_toggled(GtkToggleButton *button, LeftWin *oLeftWin);
	static void on_translate_button_toggled(GtkToggleButton *button, LeftWin *oLeftWin);
	static void PreviousCallback(GtkWidget *widget, LeftWin *oLeftWin);
	static void NextCallback(GtkWidget *widget, LeftWin *oLeftWin);
	static void on_choose_group_button_clicked(GtkWidget *widget, LeftWin *oLeftWin);
	static void on_enable_netdict_menuitem_toggled(GtkCheckMenuItem *menuitem, LeftWin *oLeftWin);
	static void on_choose_group_menuitem_toggled(GtkCheckMenuItem *menuitem, LeftWin *oLeftWin);
};

class ToolWin {
private:

	static void ShowListCallback(GtkWidget *widget, gpointer data);
	static void HideListCallback(GtkWidget *widget, gpointer data);
	static void CopyCallback(GtkWidget *widget, ToolWin *oToolWin);
	static void PlayCallback(GtkWidget *widget, ToolWin *oToolWin);
	static void SaveCallback(GtkWidget *widget, ToolWin *oToolWin);
	static void SearchCallback(GtkWidget *widget, ToolWin *oToolWin);
	static void on_pronounce_menu_item_activate(GtkMenuItem *menuitem, int engine_index);
public:
	GtkWidget* ShowListButton;
	GtkWidget* HideListButton;
	GtkToolItem* PronounceWordMenuButton;

	ToolWin(); 
	~ToolWin();
	void Create(GtkWidget *vbox);
	void UpdatePronounceMenu();
	void do_search();
	void do_save();
};

class TextWin : public sigc::trackable {
public:
  std::string queryWord;
  std::string pronounceWord;
  ReadWordType readwordtype;
  TextWinQueryResult query_result;
  std::auto_ptr<ArticleView> view;
	gboolean search_from_beginning;
	std::string find_text;
	GtkEntry *eSearch;

  TextWin() {}
  ~TextWin() {}

  void Create(GtkWidget *vbox);
  void ShowTips();
  void ShowInfo();
  void ShowInitFailed();
  void Show(const gchar *str);
  void Show(const gchar *orig_word, gchar ***Word, gchar ****WordData);
  void ShowTreeDictData(gchar *data);
  void Show(const struct STARDICT::LookupResponse::DictResponse *dict_response, STARDICT::LookupResponse::ListType list_type);
  void Show(NetDictResponse *resp);
  gboolean Find (const gchar *text, gboolean start);
	bool IsSearchPanelHasFocus() { return GTK_WIDGET_HAS_FOCUS(eSearch); } 

	void ShowSearchPanel();

private:
	GtkButton *btClose;
	GtkButton *btFind;
	GtkWidget *hbSearchPanel;
	ReadWordType selection_readwordtype;
	

	void HideSearchPanel(void) 
		{
			gtk_widget_hide_all(hbSearchPanel);
		}


	static void SelectionCallback(GtkWidget* widget,GtkSelectionData *selection_data, guint time, TextWin *oTextWin);
	static gboolean on_button_press(GtkWidget * widget, GdkEventButton * event, TextWin *oTextWin);
	static void on_query_menu_item_activate(GtkMenuItem *menuitem, TextWin *oTextWin);
	static void on_pronounce_menu_item_activate(GtkMenuItem *menuitem, TextWin *oTextWin);
	static void on_populate_popup(GtkTextView *textview, GtkMenu *menu, TextWin *oTextWin);
	static bool find_first_tag(gchar *str, gchar * & beg, gchar * & end);
	static void OnCloseSearchPanel(GtkWidget *widget, TextWin *oTextWin);
	static gboolean OnSearchKeyPress(GtkWidget *widget, GdkEventKey *event,
					 TextWin *oTextWin);
	static void OnFindSearchPanel(GtkWidget *widget, TextWin *oTextWin);
};

class TransWin {
public:
	TransWin();
	void Create(GtkWidget *notebook);
	void SetText(const char *text, int len = -1);
	bool IsInputViewHasFocus() { return GTK_WIDGET_HAS_FOCUS(input_textview); }
private:
	GtkWidget *input_textview;
	GtkWidget *result_textview;
	GtkWidget *engine_combobox;
	GtkWidget *fromlang_combobox;
	GtkWidget *tolang_combobox;
	GtkWidget *trans_button;
	GtkWidget *link_label;
	std::string pronounceWord;
	ReadWordType selection_readwordtype;
	static void on_pronounce_menu_item_activate(GtkMenuItem *menuitem, TransWin *oTransWin);
	static void on_populate_popup(GtkTextView *textview, GtkMenu *menu, TransWin *oTransWin);
	static void on_translate_button_clicked(GtkWidget *widget, TransWin *oTransWin);
	static void on_engine_combobox_changed(GtkWidget *widget, TransWin *oTransWin);
	static void on_fromlang_combobox_changed(GtkWidget *widget, TransWin *oTransWin);
	static void on_tolang_combobox_changed(GtkWidget *widget, TransWin *oTransWin);
	static void on_link_eventbox_clicked(GtkWidget *widget, GdkEventButton *event, TransWin *oTransWin);
	void SetLink(const char *linkname);
	void SetComboBox(gint engine_index, gint fromlang_index, gint tolang_index);
	void GetHostFile(std::string &host, std::string &file, const char *text);
};

class MidWin {
public:
	GtkWidget* hpaned;
	GtkWidget* notebook;

	LeftWin oLeftWin;
	IndexWin oIndexWin;
	ToolWin oToolWin;
	TextWin oTextWin;
	TransWin oTransWin;

	MidWin() {}
	void Create(GtkWidget *vbox);
};

class BottomWin {
private:
	GtkWidget *movenews_event_box;
	GtkWidget *news_label;
	GtkWidget *link_hbox;
	GtkWidget *link_label;
	int news_timeout_id;
	int link_timeout_id;
	size_t news_move_index;
	size_t link_index;
	size_t news_move_len;
	bool need_resume_news;
	std::string news_text;
	std::vector<std::pair<std::string, std::string> > linklist;
	static void ScanCallback(GtkToggleButton *button, gpointer data);
	static void AboutCallback(GtkButton *button, gpointer data);
	static void QuitCallback(GtkButton *button, gpointer data);
	static void InternetSearchCallback(GtkButton *button, BottomWin *oBottomWin);
	static void NewVersionCallback(GtkButton *button, BottomWin *oBottomWin);
	static void DictManageCallback(GtkButton *button, BottomWin *oBottomWin);
	static void PreferenceCallback(GtkButton *button, BottomWin *oBottomWin);
	static gboolean on_internetsearch_button_press(GtkWidget * widget, GdkEventButton * event , BottomWin *oBottomWin);
	static void on_internetsearch_menu_item_activate(GtkMenuItem *menuitem, const gchar *website);
	static gboolean move_news(gpointer data);
	static gboolean change_link(gpointer data);
	static void on_link_eventbox_clicked(GtkWidget *widget, GdkEventButton *event, BottomWin *oBottomWin);
	static gboolean vEnterNotifyCallback (GtkWidget *widget, GdkEventCrossing *event, BottomWin *oBottomWin);
	static gboolean vLeaveNotifyCallback (GtkWidget *widget, GdkEventCrossing *event, BottomWin *oBottomWin);
public:
	GtkWidget* ScanSelectionCheckButton;
	GtkWidget* SearchWebsiteMenu;

	BottomWin();
	void Destroy();
	void Create(GtkWidget *vbox);
	void set_news(const char *news, const char *links);
	static void InternetSearch(const std::string& website);
};

#endif
