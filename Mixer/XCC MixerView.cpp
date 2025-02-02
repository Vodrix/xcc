#include "stdafx.h"
#include "XCC Mixer.h"
#include "XSE_dlg.h"
#include "MainFrm.h"
#include "XCCFileView.h"
#include "XCC MixerDoc.h"
#include "XCC MixerView.h"
#include "resource.h"
#include "XSTE_dlg.h"
#include <aud_decode.h>
#include <aud_file_write.h>
#include <aud_file.h>
#include <big_edit.h>
#include <big_file_write.h>
#include <cps_file.h>
#include <dds_file.h>
#include <fstream>
#include <hva_file.h>
#include <id_log.h>
#include <ima_adpcm_wav_decode.h>
#include <ima_adpcm_wav_encode.h>
#include <image_tools.h>
#include <jpeg_file.h>
#include <map_ts_encoder.h>
#include <map_ts_ini_reader.h>
#include <mix_edit.h>
#include <mix_file_write.h>
#include <mix_rg_edit.h>
#include <mix_rg_file_write.h>
#include <pal_file.h>
#include <pal_file.h>
#include <pcx_decode.h>
#include <pcx_file.h>
#include <pkt_ts_ini_reader.h>
#include <shp_decode.h>
#include <shp_dune2_file.h>
#include <shp_file.h>
#include <shp_images.h>
#include <shp_ts_file_write.h>
#include <shp_ts_file.h>
#include <sstream>
#include <st_file.h>
#include <string_conversion.h>
#include <text_file.h>
#include <tmp_file.h>
#include <tmp_ra_file.h>
#include <tmp_ts_file.h>
#include <virtual_tfile.h>
#include <voc_file.h>
#include <vqa_file.h>
#include <vxl_file.h>
#include <wav_file.h>
#include <wav_structures.h>
#include <wsa_dune2_file.h>
#include <wsa_file.h>
#include <xcc_dirs.h>
#include <xif_file.h>
#include "dlg_shp_viewer.h"
#include "resizedlg.h"
#include "shp_properties_dlg.h"
#include "shortcut.h"

IMPLEMENT_DYNCREATE(CXCCMixerView, CListView)

BEGIN_MESSAGE_MAP(CXCCMixerView, CListView)
	ON_COMMAND_RANGE(ID_FILE_FOUND_MIX000, ID_FILE_FOUND_MIX199, OnFileFound)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CXCCMixerView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_COMMAND(ID_POPUP_EXTRACT, OnPopupExtract)
	ON_UPDATE_COMMAND_UI(ID_POPUP_EXTRACT, OnUpdatePopupExtract)
	ON_COMMAND(ID_POPUP_COPY, OnPopupCopy)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY, OnUpdatePopupCopy)
	ON_COMMAND(ID_POPUP_COPY_AS_AUD, OnPopupCopyAsAUD)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_AUD, OnUpdatePopupCopyAsAUD)
	ON_COMMAND(ID_POPUP_COPY_AS_AVI, OnPopupCopyAsAVI)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_AVI, OnUpdatePopupCopyAsAVI)
	ON_COMMAND(ID_POPUP_COPY_AS_CPS, OnPopupCopyAsCPS)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_CPS, OnUpdatePopupCopyAsCPS)
	ON_COMMAND(ID_POPUP_COPY_AS_PCX, OnPopupCopyAsPCX)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_PCX, OnUpdatePopupCopyAsPCX)
	ON_COMMAND(ID_POPUP_COPY_AS_SHP, OnPopupCopyAsSHP)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_SHP, OnUpdatePopupCopyAsSHP)
	ON_COMMAND(ID_POPUP_COPY_AS_WSA, OnPopupCopyAsWSA)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_WSA, OnUpdatePopupCopyAsWSA)
	ON_COMMAND(ID_POPUP_DELETE, OnPopupDelete)
	ON_UPDATE_COMMAND_UI(ID_POPUP_DELETE, OnUpdatePopupDelete)
	ON_COMMAND(ID_POPUP_OPEN, OnPopupOpen)
	ON_UPDATE_COMMAND_UI(ID_POPUP_OPEN, OnUpdatePopupOpen)
	ON_COMMAND(ID_POPUP_COPY_AS_VXL, OnPopupCopyAsVXL)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_VXL, OnUpdatePopupCopyAsVXL)
	ON_COMMAND(ID_POPUP_COPY_AS_XIF, OnPopupCopyAsXIF)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_XIF, OnUpdatePopupCopyAsXIF)
	ON_COMMAND(ID_POPUP_COPY_AS_CSV, OnPopupCopyAsCSV)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_CSV, OnUpdatePopupCopyAsCSV)
	ON_COMMAND(ID_POPUP_COPY_AS_HVA, OnPopupCopyAsHVA)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_HVA, OnUpdatePopupCopyAsHVA)
	ON_COMMAND(ID_POPUP_COPY_AS_PAL, OnPopupCopyAsPAL)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_PAL, OnUpdatePopupCopyAsPAL)
	ON_COMMAND(ID_POPUP_COPY_AS_SHP_TS, OnPopupCopyAsSHP_TS)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_SHP_TS, OnUpdatePopupCopyAsSHP_TS)
	ON_COMMAND(ID_POPUP_COPY_AS_PAL_JASC, OnPopupCopyAsPAL_JASC)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_PAL_JASC, OnUpdatePopupCopyAsPAL_JASC)
	ON_COMMAND(ID_POPUP_COPY_AS_TEXT, OnPopupCopyAsText)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_TEXT, OnUpdatePopupCopyAsText)
	ON_COMMAND(ID_POPUP_COPY_AS_MAP_TS_PREVIEW, OnPopupCopyAsMapTsPreview)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_MAP_TS_PREVIEW, OnUpdatePopupCopyAsMapTsPreview)
	ON_COMMAND(ID_POPUP_REFRESH, OnPopupRefresh)
	ON_UPDATE_COMMAND_UI(ID_POPUP_REFRESH, OnUpdatePopupRefresh)
	ON_COMMAND(ID_POPUP_RESIZE, OnPopupResize)
	ON_UPDATE_COMMAND_UI(ID_POPUP_RESIZE, OnUpdatePopupResize)
	ON_COMMAND(ID_POPUP_COPY_AS_HTML, OnPopupCopyAsHTML)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_HTML, OnUpdatePopupCopyAsHTML)
	ON_COMMAND(ID_POPUP_COPY_AS_PNG, OnPopupCopyAsPNG)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_PNG, OnUpdatePopupCopyAsPNG)
	ON_COMMAND(ID_POPUP_COPY_AS_WAV_IMA_ADPCM, OnPopupCopyAsWavImaAdpcm)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_WAV_IMA_ADPCM, OnUpdatePopupCopyAsWavImaAdpcm)
	ON_COMMAND(ID_POPUP_COPY_AS_WAV_PCM, OnPopupCopyAsWavPcm)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_WAV_PCM, OnUpdatePopupCopyAsWavPcm)
	ON_COMMAND(ID_POPUP_COPY_AS_PCX_SINGLE, OnPopupCopyAsPcxSingle)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_PCX_SINGLE, OnUpdatePopupCopyAsPcxSingle)
	ON_COMMAND(ID_EDIT_COPY, OnPopupClipboardCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdatePopupClipboardCopy)
	ON_COMMAND(ID_POPUP_COPY_AS_PNG_SINGLE, OnPopupCopyAsPngSingle)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_PNG_SINGLE, OnUpdatePopupCopyAsPngSingle)
	ON_COMMAND(ID_POPUP_CLIPBOARD_PASTE_AS_PCX, OnPopupClipboardPasteAsPcx)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLIPBOARD_PASTE_AS_PCX, OnUpdatePopupClipboardPasteAsImage)
	ON_COMMAND(ID_POPUP_CLIPBOARD_PASTE_AS_SHP_TS, OnPopupClipboardPasteAsShpTs)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLIPBOARD_PASTE_AS_SHP_TS, OnUpdatePopupClipboardPasteAsVideo)
	ON_COMMAND(ID_POPUP_CLIPBOARD_PASTE_AS_PNG, OnPopupClipboardPasteAsPng)
	ON_COMMAND(ID_POPUP_COPY_AS_JPEG, OnPopupCopyAsJpeg)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_JPEG, OnUpdatePopupCopyAsJpeg)
	ON_COMMAND(ID_POPUP_CLIPBOARD_PASTE_AS_JPEG, OnPopupClipboardPasteAsJpeg)
	ON_COMMAND(ID_POPUP_EXPLORE, OnPopupExplore)
	ON_UPDATE_COMMAND_UI(ID_POPUP_EXPLORE, OnUpdatePopupExplore)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_POPUP_COMPACT, OnPopupCompact)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COMPACT, OnUpdatePopupCompact)
	ON_COMMAND(ID_POPUP_COPY_AS_TGA, OnPopupCopyAsTga)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_TGA, OnUpdatePopupCopyAsTga)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_POPUP_CLIPBOARD_PASTE_AS_TGA, OnPopupClipboardPasteAsTga)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLIPBOARD_PASTE_AS_PNG, OnUpdatePopupClipboardPasteAsImage)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLIPBOARD_PASTE_AS_JPEG, OnUpdatePopupClipboardPasteAsImage)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLIPBOARD_PASTE_AS_TGA, OnUpdatePopupClipboardPasteAsImage)
	ON_COMMAND(ID_POPUP_COPY_AS_JPEG_SINGLE, OnPopupCopyAsJpegSingle)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_JPEG_SINGLE, OnUpdatePopupCopyAsJpegSingle)
	ON_COMMAND(ID_POPUP_COPY_AS_TGA_SINGLE, OnPopupCopyAsTgaSingle)
	ON_UPDATE_COMMAND_UI(ID_POPUP_COPY_AS_TGA_SINGLE, OnUpdatePopupCopyAsTgaSingle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

enum t_error_message
{
	em_bad_fname = 0x400,
	em_bad_depth,
	em_bad_size
};

const char* error_messages[] =
{
	"Bad filename. All files should have a name like \"image ####.pcx\" where #### is the zero based index.",
	"Bad color depth. Only 8-bit images are supported.",
	"Bad size. All images should have the same size."
};

static CXCCMixerApp* GetApp()
{
	return static_cast<CXCCMixerApp*>(AfxGetApp());
}

static CMainFrame* GetMainFrame()
{
	return static_cast<CMainFrame*>(AfxGetMainWnd());
}

CXCCMixerView::CXCCMixerView()
{
}

CXCCMixerView::~CXCCMixerView()
{
}

BOOL CXCCMixerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS;
	return CListView::PreCreateWindow(cs);
}

void CXCCMixerView::OnInitialUpdate()
{
	char dir[MAX_PATH];
	if (GetCurrentDirectory(MAX_PATH, dir))
	{
		Cfname fn(dir);
		fn.make_path();
		m_dir = fn.get_all();
	}
	else
		m_dir = "c:\\";
	{
		string dir(AfxGetApp()->GetProfileString(m_reg_key, "path", m_dir.c_str()));
		string fname = dir + "write_check.temp";
		if (!file32_write(fname, NULL, 0))
		{
			delete_file(fname);
			m_dir = dir;
		}
	}

	CListView::OnInitialUpdate();

	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	GetListCtrl().InsertColumn(0, "Name", LVCFMT_LEFT);
	GetListCtrl().InsertColumn(1, "Type", LVCFMT_LEFT);
	GetListCtrl().InsertColumn(2, "Size", LVCFMT_RIGHT);
	GetListCtrl().InsertColumn(3, "Description", LVCFMT_LEFT);
	update_list();
}

void CXCCMixerView::OnFileNew()
{
	const char* save_filter = "TD/RA MIX (*.mix)|*.mix|TS/RA2 MIX (*.mix)|*.mix|Renegade MIX (*.mix)|*.mix|Generals BIG (*.big)|*.big|";

	close_all_locations();
	CFileDialog dlg(false, save_filter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, save_filter, this);
	dlg.m_ofn.nFilterIndex = 2;
	if (IDOK == dlg.DoModal())
	{
		string name(dlg.GetPathName());
		int error = 0;
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			error = Cmix_file_write(game_ra).write().save(name);
			break;
		case 2:
			error = Cmix_file_write(game_ts).write().save(name);
			break;
		//case 3:	//TS mixes are the same as ra2 ones
		//	error = Cmix_file_write(game_ra2).write().save(name);
		//	break;
		case 3:
			error = Cmix_rg_file_write().write().save(name);
			break;
		case 4:
			error = Cbig_file_write().write().save(name);
			break;
		default:
			assert(false);
		}
		//update_list();
		close_all_locations();
		open_location_mix(static_cast<string>(dlg.GetPathName()));
	}
}

void CXCCMixerView::OnFileOpen()
{
	CFileDialog dlg(true, "mix", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "Archive files (*.big;*.dat;*.mix;*.pkg;*.wsx;*.pak)|*.big;*.dat;*.mix;*.pkg;*.wsx;*.pak;|", this);
	if (IDOK == dlg.DoModal())
	{
		close_all_locations();
		open_location_mix(static_cast<string>(dlg.GetPathName()));
	}
}

void CXCCMixerView::OnFileClose()
{
	if (m_mix_f)
		close_location(true);
	else
	{
		int i = m_dir.rfind('\\');
		if (i != string::npos)
		{
			i = m_dir.rfind('\\', i - 1);
			if (i != string::npos)
				open_location_dir(m_dir.substr(0, i + 1));
		}
	}
}

void CXCCMixerView::open_location_dir(const string& name)
{
	m_dir = name;
	update_list();
}

void CXCCMixerView::open_location_mix(const string& name)
{
	Cmix_file* mix_f = new Cmix_file;
	if (mix_f->open(name))
	{
		delete mix_f;
		mix_f = new Cmix_file_rd;
		if (static_cast<Cmix_file_rd*>(mix_f)->open(name))
		{
			delete mix_f;
		}
		else
		{
			goto MPUSH;
		}
	}
	else
	{
	MPUSH:
		m_location.push(m_mix_f);
		m_mix_f = mix_f;
		m_mix_fname = name;
	}
	update_list();
}

void CXCCMixerView::open_location_mix(t_mix_map_list::const_iterator i, int file_id)
{
	using t_stack = stack<int>;
	t_stack stack;
	close_all_locations();
	const t_mix_map_list& mix_list = GetMainFrame()->mix_map_list();
	while (i->second.fname.empty())
	{
		stack.push(i->second.id);
		i = mix_list.find(i->second.parent);
	}
	open_location_mix(i->second.fname);
	while (!stack.empty())
	{
		open_location_mix(stack.top());
		stack.pop();
	}
	if (file_id)
	{
		CListCtrl& lc = GetListCtrl();
		LVFINDINFO lvf;
		lvf.flags = LVFI_PARAM;
		lvf.lParam = file_id;
		int i = lc.FindItem(&lvf, -1);
		if (i != -1)
		{
			lc.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			lc.EnsureVisible(i, false);
		}
	}
}

void CXCCMixerView::open_location_mix(int mix_id, int sub_mix_id, int file_id)
{
	close_all_locations();
	const t_index_entry& mix = t_index_list().at(mix_id);
	open_location_mix(m_dir.rfind('\\') == string::npos ? (m_dir + '\\' + mix.name) : (m_dir + mix.name));
	if (sub_mix_id >= 0)
	{
		open_location_mix(m_mix_f->get_id(sub_mix_id));
	}
	if (file_id)
	{
		CListCtrl& lc = GetListCtrl();
		LVFINDINFO lvf;
		lvf.flags = LVFI_PARAM;
		lvf.lParam = file_id;
		int i = lc.FindItem(&lvf, -1);
		if (i != -1)
		{
			lc.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			lc.EnsureVisible(i, false);
		}
	}
}

void CXCCMixerView::open_location_mix(int id)
{
	Cmix_file* mix_f = new Cmix_file;
	if (mix_f->open(id, *m_mix_f))
	{
		delete mix_f;
		mix_f = new Cmix_file_rd;
		if (static_cast<Cmix_file_rd*>(mix_f)->open(id, *m_mix_f))
		{
			delete static_cast<Cmix_file_rd*>(mix_f);
		}
		else
		{
			goto MPUSH;
		}
	}
	else
	{
	MPUSH:
		m_location.push(m_mix_f);
		m_mix_f = mix_f;
		update_list();
	}
}

void CXCCMixerView::close_location(int reload)
{
	if (m_mix_f)
	{
		delete m_mix_f;
		m_mix_f = m_location.top();
		m_location.pop();
		if (reload)
			update_list();
	}
}

void CXCCMixerView::close_all_locations()
{
	while (!m_location.empty())
		close_location(false);
}

void CXCCMixerView::clear_list()
{
	GetListCtrl().DeleteAllItems();
	m_index.clear();
}

string totalSize(size_t i)
{
	static const char* sizenames[] = { " B", " KB", " MB", " GB" };
	size_t div = 0;
	size_t rem = 0;
	while (i >= 1024 && div < (sizeof sizenames / sizeof * sizenames))
	{
		rem = (i % 1024);
		div++;
		i /= 1024;
	}
	return n((float)i + (float)rem / 1024.0) + sizenames[div];
}

void CXCCMixerView::update_list()
{
	DragAcceptFiles(can_edit());
	clear_list();
	t_index_entry e;
	m_palette_loaded = false;

	if (m_mix_f)
	{
		e.name = "..";
		e.ft = ft_dir;
		e.size = "";
		e.description = "";
		m_index[0] = e;

		m_game = m_mix_f->get_game();
		for (int i = 0; i < m_mix_f->get_c_files(); i++)
		{
			int id = m_mix_f->get_id(i);
			e.description = mix_database::get_description(m_game, id);
			e.ft = m_mix_f->get_type(id);
			e.name = mix_database::get_name(m_game, id);
			if (e.name.empty())
				e.name = nh(8, id);
			e.size = totalSize(m_mix_f->get_size(id));
			m_index[id] = e;
			if (e.ft == ft_pal)
			{
				m_mix_f->get_vdata(id).read(m_palette);
				m_palette_loaded = true;
			}
		}
	}
	else
	{
		m_game = game_ts;
		e.name = "Browse...";
		e.ft = ft_drive;
		e.size = "";
		e.description = "";
		m_index[0] = e;
		//int drivemap = GetLogicalDrives();
		//char drive_name[] = "a:\\";
		//for (int i = 0; i < 26; i++)
		//{
		//	if (drivemap >> i & 1)
		//	{
		//		e.name = drive_name;
		//		e.ft = ft_drive;
		//		e.size = "";
		//		e.description = "";
		//		m_index[Cmix_file::get_id(get_game(), e.name)] = e;
		//	}
		//	drive_name[0]++;
		//}
		WIN32_FIND_DATA finddata;
		HANDLE findhandle = FindFirstFile((m_dir + "*").c_str(), &finddata);
		if (findhandle != INVALID_HANDLE_VALUE)
		{
			Ccc_file f(false);
			do
			{
				e.name = finddata.cFileName;
				e.size = "";
				if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (e.name == ".")
						continue;
					e.ft = ft_dir;
				}
				else
					e.ft = static_cast<t_file_type>(-1);
				int id = Cmix_file::get_id(get_game(), finddata.cFileName);
				e.description = mix_database::get_description(get_game(), id);
				m_index[id] = e;
			}
			while (FindNextFile(findhandle, &finddata));
			FindClose(findhandle);
		}
	}
	SetRedraw(false);
	CListCtrl& lc = GetListCtrl();
	for (auto& i : m_index)
		lc.SetItemData(lc.InsertItem(lc.GetItemCount(), LPSTR_TEXTCALLBACK), i.first);
	sort_list(0, false);
	sort_list(1, false);
	SetRedraw(true);
	autosize_colums();
	m_reading = true;
}

void CXCCMixerView::autosize_colums()
{
	SetRedraw(false);
	for (int i = 0; i < GetListCtrl().GetHeaderCtrl()->GetItemCount(); i++)
		GetListCtrl().SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	SetRedraw(true);
}

void CXCCMixerView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	m_buffer[++m_buffer_w &= 3].erase();
	const t_index_entry& e = find_ref(m_index, pDispInfo->item.lParam);
	switch (pDispInfo->item.iSubItem)
	{
	case 0:
		m_buffer[m_buffer_w] = e.name;
		break;
	case 1:
		if (e.ft != -1)
			m_buffer[m_buffer_w] = ft_name[e.ft];
		break;
	case 2:
		m_buffer[m_buffer_w] = e.size;
		break;
	case 3:
		m_buffer[m_buffer_w] = e.description;
		break;
	}
	pDispInfo->item.pszText = const_cast<char*>(m_buffer[m_buffer_w].c_str());
	*pResult = 0;
}

template <class T>
static int compare(const T& a, const T& b)
{
	return a < b ? -1 : a != b;
}

int CXCCMixerView::compare(int id_a, int id_b) const
{
	if (m_sort_reverse)
		swap(id_a, id_b);
	const t_index_entry& a = find_ref(m_index, id_a);
	const t_index_entry& b = find_ref(m_index, id_b);
	if (a.ft != b.ft)
	{
		if (a.ft == ft_drive)
			return -1;
		if (b.ft == ft_drive)
			return 1;
		if (a.ft == ft_dir || a.ft == ft_lnkdir)
			return -1;
		if (b.ft == ft_dir || b.ft == ft_lnkdir)
			return 1;
	}
	switch (m_sort_column)
	{
	case 0:
		return ::compare(to_lower(a.name), to_lower(b.name));
	case 1:
		return ::compare(a.ft, b.ft);
	case 2:
		return ::compare(b.size, a.size);
	case 3:
		return ::compare(a.description, b.description);
	}
	return 0;
}

static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return reinterpret_cast<CXCCMixerView*>(lParamSort)->compare(lParam1, lParam2);
}

void CXCCMixerView::sort_list(int i, bool reverse)
{
	m_sort_column = i;
	m_sort_reverse = reverse;
	GetListCtrl().SortItems(Compare, reinterpret_cast<DWORD_PTR>(this));
}

void CXCCMixerView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	int column = reinterpret_cast<NM_LISTVIEW*>(pNMHDR)->iSubItem;
	sort_list(column, column == m_sort_column ? !m_sort_reverse : false);
	*pResult = 0;
}

void CXCCMixerView::OnDestroy()
{
	AfxGetApp()->WriteProfileString(m_reg_key, "path", m_dir.c_str());
	close_all_locations();
}

void CXCCMixerView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	int id = get_current_id();
	if (id != -1)
		open_item(id);
	*pResult = 0;
}

void CXCCMixerView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);
	if (pNMListView->uNewState & LVIS_FOCUSED)
	{
		LV_ITEM lvi;
		lvi.mask = LVIF_PARAM;
		lvi.iItem = pNMListView->iItem;
		GetListCtrl().GetItem(&lvi);
		if (m_mix_f)
			m_file_view_pane->open_f(lvi.lParam, *m_mix_f, m_game, m_palette_loaded ? m_palette : NULL);
		else
		{
			const t_index_entry& index = m_index[lvi.lParam];
			if (index.ft != ft_dir && index.ft != ft_drive && index.ft != ft_lnkdir)
				m_file_view_pane->open_f(m_dir + index.name);
		}
	}
	*pResult = 0;
}

int CXCCMixerView::get_current_id() const
{
	int i = get_current_index();
	return i == -1 ? -1 : get_id(i);
}

int CXCCMixerView::get_current_index() const
{
	return GetListCtrl().GetNextItem(-1, LVNI_ALL | LVNI_FOCUSED);
}

int CXCCMixerView::get_id(int i) const
{
	return GetListCtrl().GetItemData(i);
}

void CXCCMixerView::set_other_panes(CXCCFileView* file_view_pane, CXCCMixerView* other_pane)
{
	m_file_view_pane = file_view_pane;
	m_other_pane = other_pane;
}

void CXCCMixerView::OnContextMenu(CWnd*, CPoint point)
{
		if (point.x == -1 && point.y == -1)
		{
			// keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_MIX_VIEW));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
}

void CXCCMixerView::OnFileFound(UINT ID)
{
	close_all_locations();
	open_location_mix(GetMainFrame()->get_mix_name(ID - ID_FILE_FOUND_MIX000));
}

int CXCCMixerView::open_f_id(Ccc_file& f, int id) const
{
	return m_mix_f ? f.open(id, *m_mix_f) : f.open(m_dir + find_ref(m_index, id).name);
}

int CXCCMixerView::open_f_index(Ccc_file& f, int i) const
{
	return open_f_id(f, GetListCtrl().GetItemData(i));
}

Cvirtual_binary CXCCMixerView::get_vdata_id(int id) const
{
	return m_mix_f ? m_mix_f->get_vdata(id) : Cvirtual_binary(m_dir + find_ref(m_index, id).name);
}

Cvirtual_binary CXCCMixerView::get_vdata(int i) const
{
	return get_vdata_id(GetListCtrl().GetItemData(i));
}

Cvirtual_image CXCCMixerView::get_vimage_id(int id) const
{
	Cvirtual_image d;
	switch (find_ref(m_index, id).ft)
	{
	case ft_cps:
		{
			Ccps_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
		}
		break;
	case ft_dds:
		{
			Cdds_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
			d.remove_alpha();
		}
		break;
	case ft_jpeg:
	case ft_pcx:
	case ft_png:
	case ft_tga:
		d.load(get_vdata_id(id));
		break;
	case ft_shp:
		{
			Cshp_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
		}
		break;
	case ft_shp_ts:
		{
			Cshp_ts_file f;
			f.load(get_vdata_id(id));
			d = f.extract_as_pcx_single(get_default_palette(), GetMainFrame()->combine_shadows());
		}
		break;
	case ft_tmp_ra:
		{
			Ctmp_ra_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
		}
		break;
	case ft_tmp_ts:
		{
			Ctmp_ts_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
		}
		break;
	case ft_wsa_dune2:
		{
			Cwsa_dune2_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
		}
		break;
	case ft_wsa:
		{
			Cwsa_file f;
			f.load(get_vdata_id(id));
			d = f.vimage();
		}
		break;
	}
	if (d.cb_pixel() == 1 && !d.palette())
		d.palette(get_default_palette(), true);
	return d;
}

Cvirtual_image CXCCMixerView::get_vimage(int i) const
{
	return get_vimage_id(GetListCtrl().GetItemData(i));
}

void CXCCMixerView::OnPopupExtract()
{
	int id = get_current_id();
	CFileDialog dlg(false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, "All files (*.*)|*.*|", this);
	char s[MAX_PATH];
	strcpy(s, find_ref(m_index, id).name.c_str());
	dlg.m_ofn.lpstrFile = s;
	if (IDOK != dlg.DoModal())
		return;
	Ccc_file f(false);
	if (!open_f_index(f, get_current_index()))
		f.extract(static_cast<string>(dlg.GetPathName()));
}

void CXCCMixerView::OnUpdatePopupExtract(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(get_current_id() != -1);
}

const t_palette_entry* CXCCMixerView::get_default_palette() const
{
	const t_palette_entry* p = GetMainFrame()->get_pal_data();
	return p ? p : m_palette;
}

bool CXCCMixerView::can_accept() const
{
	//return !m_mix_f;
	return true;
}

bool CXCCMixerView::can_edit() const
{
	return m_location.size() < 2;
}

string CXCCMixerView::get_dir() const
{
	return m_dir;
}

void CXCCMixerView::set_reg_key(const string& v)
{
	m_reg_key = v.c_str();
}

static bool can_convert(t_file_type s, t_file_type d)
{
	switch (s)
	{
	case ft_aud:
	case ft_voc:
		return d == ft_wav_pcm;
	case ft_cps:
	case ft_dds:
	case ft_jpeg:
	case ft_pcx:
	case ft_png:
	case ft_tga:
	case ft_shp:
	case ft_tmp_ra:
	case ft_tmp_ts:
	case ft_wsa_dune2:
	case ft_wsa:
		return d == ft_clipboard
			|| d == ft_cps
			|| d == ft_jpeg
			|| d == ft_map_ts_preview
			|| d == ft_pal
			|| d == ft_pal_jasc
			|| d == ft_pcx
			|| d == ft_png
			|| d == ft_shp
			|| d == ft_shp_ts
			|| d == ft_tga
			|| d == ft_vxl;
	case ft_hva:
		return d == ft_csv;
	case ft_pal:
		return d == ft_pal_jasc
			|| d == ft_text;
	case ft_mix:
	case ft_st:
		return d == ft_text;
	case ft_shp_dune2:
		return d == ft_jpeg
			|| d == ft_pcx
			|| d == ft_png
			|| d == ft_tga;
	case ft_shp_ts:
		return d == ft_clipboard
			|| d == ft_jpeg
			|| d == ft_jpeg_single
			|| d == ft_pcx
			|| d == ft_pcx_single
			|| d == ft_png
			|| d == ft_png_single
			|| d == ft_tga
			|| d == ft_tga_single;
	case ft_text:
		return d == ft_html
			|| d == ft_hva
			|| d == ft_vxl;
	case ft_vqa:
		return d == ft_avi
			|| d == ft_jpeg
			|| d == ft_pcx
			|| d == ft_png
			|| d == ft_tga
			|| d == ft_wav_pcm;
	case ft_vxl:
		return d == ft_jpeg
			|| d == ft_pcx
			|| d == ft_png
			|| d == ft_text
			|| d == ft_tga
			|| d == ft_xif;
	case ft_wav:
		return d == ft_aud
			|| d == ft_wav_ima_adpcm
			|| d == ft_wav_pcm;
	case ft_xif:
		return d == ft_html
			|| d == ft_text
			|| d == ft_vxl;
	}
	return false;
}

bool CXCCMixerView::can_copy()
{
	return can_delete() && m_other_pane->can_accept();
}

bool CXCCMixerView::can_delete()
{
	m_index_selected.clear();
	int i = GetListCtrl().GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	while (i != -1)
	{
		int id = GetListCtrl().GetItemData(i);
		const t_index_entry& index = find_ref(m_index, id);
		if (index.ft == ft_drive || index.name.empty() || (index.ft == ft_dir && (index.name == "." || index.name == "..")))
			return false;
		m_index_selected.push_back(i);
		i = GetListCtrl().GetNextItem(i, LVNI_ALL | LVNI_SELECTED);
	}
	return true;
}

bool CXCCMixerView::can_copy_as(t_file_type ft)
{
	if (m_other_pane->m_mix_f || m_mix_f || m_index_selected.empty())
		return false;
	for (auto& i : m_index_selected)
	{
		if (!can_convert(find_ref(m_index, get_id(i)).ft, ft))
			return false;
	}
	return true;
}

static void set_msg(const string& s)
{
	GetMainFrame()->SetMessageText(s.c_str());
}

static void copy_failed(const Cfname& fname, int error)
{
	string v = "Copy " + fname.get_ftitle() + " failed, error " + n(error);
	if (error & 0x400)
	{
		v += ": ";
		v += error_messages[error & 0x3ff];
	}
	set_msg(v);
}

static void copy_succeeded(const Cfname& fname)
{
	set_msg("Copy " + fname.get_ftitle() + " succeeded");
}

int big_insert_dir(Cbig_edit& f, const string& dir, const string& name_prefix)
{
	int error = 0;
	WIN32_FIND_DATA fd;
	HANDLE fh = FindFirstFile((dir + '*').c_str(), &fd);
	if (fh != INVALID_HANDLE_VALUE)
	{
		do
		{
			string fname = dir + fd.cFileName;
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (*fd.cFileName != '.')
					error = error ? big_insert_dir(f, fname + '/', name_prefix + fd.cFileName + '\\'), error : big_insert_dir(f, fname + '/', name_prefix + fd.cFileName + '\\');
			}
			else
			{
				Cvirtual_binary d;
				if (!d.load(fname))
					error = error ? f.insert(name_prefix + fd.cFileName, d) : f.insert(name_prefix + fd.cFileName, d), error;
			}
		} while (FindNextFile(fh, &fd));
		FindClose(fh);
	}
	return error;
}

void CXCCMixerView::copy_as(t_file_type ft)
{
	CWaitCursor wait;
	int error;
	for (auto& i : m_index_selected)
	{
		const Cfname fname = m_other_pane->get_dir() + find_ref(m_index, get_id(i)).name;
		if (find_ref(m_index, get_id(i)).name.find('\\') != string::npos)
			create_deep_dir(m_other_pane->get_dir(), Cfname(find_ref(m_index, get_id(i)).name).get_path());
		if (m_other_pane->m_mix_f)
		{
			const Cfname fname = get_dir() + find_ref(m_index, get_id(i)).name;
			if (find_ref(m_index, get_id(i)).name.find('\\') != string::npos)
				create_deep_dir(get_dir(), Cfname(find_ref(m_index, get_id(i)).name).get_path());
			t_file_type mix_ft = m_other_pane->m_mix_f->get_file_type();
			m_other_pane->close_location(false);
			switch (mix_ft)
			{
			case ft_big:
				{
					Cbig_edit f;
					error = f.open(m_other_pane->m_mix_fname);
					if (!error)
					{
						Cvirtual_binary d;
						DWORD file_attributes = GetFileAttributes(fname.get_all().c_str());
						if (file_attributes == INVALID_FILE_ATTRIBUTES || ~file_attributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							Cvirtual_binary d;
							if (!d.load(fname))
								error = error ? f.insert(Cfname(fname).get_fname(), d) : f.insert(Cfname(fname).get_fname(), d), error;
						}
						else
							error = error ? big_insert_dir(f, string(fname) + '/', Cfname(fname).get_fname() + '\\'), error : big_insert_dir(f, string(fname) + '/', Cfname(fname).get_fname() + '\\');

						error = error ? f.write_index(), error : f.write_index();
						f.close();
					}
					break;
				}
			case ft_mix_rg:
				{
					Cmix_rg_edit f;
					error = f.open(m_other_pane->m_mix_fname);
					if (!error)
					{
						Cvirtual_binary d;
						if (!d.load(fname))
							error = error ? f.insert(Cfname(fname).get_fname(), d) : f.insert(Cfname(fname).get_fname(), d), error;
						error = error ? f.write_index(), error : f.write_index();
						f.close();
					}
					break;
				}
			default:
				{
					Cmix_edit f;
					error = f.open(m_other_pane->m_mix_fname);
					if (!error)
					{
						Cvirtual_binary d;
						if (!d.load(fname))
							error = error ? f.insert(Cfname(fname).get_fname(), d) : f.insert(Cfname(fname).get_fname(), d), error;
						error = error ? f.write_index(), error : f.write_index();
						f.close();
					}
				}
			}
			m_other_pane->open_location_mix(m_other_pane->m_mix_fname);
			if (error)
				copy_failed(fname, error);
			else
				copy_succeeded(fname);
		}
		else
		{
			switch (ft)
			{
			case -1:
				error = copy(i, fname);
				break;
			case ft_aud:
				error = copy_as_aud(i, fname);
				break;
			case ft_avi:
				error = copy_as_avi(i, fname);
				break;
			case ft_cps:
				error = copy_as_cps(i, fname);
				break;
			case ft_csv:
				error = copy_as_csv(i, fname);
				break;
			case ft_html:
				error = copy_as_html(i, fname);
				break;
			case ft_hva:
				error = copy_as_hva(i, fname);
				break;
			case ft_jpeg:
			case ft_jpeg_single:
			case ft_pcx:
			case ft_pcx_single:
			case ft_png:
			case ft_png_single:
			case ft_tga:
			case ft_tga_single:
				error = copy_as_pcx(i, fname, ft);
				break;
			case ft_map_ts_preview:
				error = copy_as_map_ts_preview(i, fname);
				break;
			case ft_pal:
				error = copy_as_pal(i, fname);
				break;
			case ft_pal_jasc:
				error = copy_as_pal_jasc(i, fname);
				break;
			case ft_shp:
				error = copy_as_shp(i, fname);
				break;
			case ft_shp_ts:
				error = copy_as_shp_ts(i, fname);
				break;
			case ft_text:
				error = copy_as_text(i, fname);
				break;
			case ft_vxl:
				error = copy_as_vxl(i, fname);
				break;
			case ft_wav_ima_adpcm:
				error = copy_as_wav_ima_adpcm(i, fname);
				break;
			case ft_wav_pcm:
				error = copy_as_wav_pcm(i, fname);
				break;
			case ft_xif:
				error = copy_as_xif(i, fname);
				break;
			default:
				error = 1;
			}
			if (error)
				copy_failed(fname, error);
			else
				copy_succeeded(fname);
		}
	}
	if (m_dir == m_other_pane->m_dir && !m_mix_f)
		update_list();
	m_other_pane->update_list();
	if (m_other_pane->m_mix_f)
		m_other_pane->OnPopupCompact();
}

int CXCCMixerView::copy(int i, Cfname fname) const
{
	Ccc_file f(false);
	int error = open_f_index(f, i);
	return error ? error : f.extract(fname);
}

int CXCCMixerView::copy_as_aud(int i, Cfname fname) const
{
	Cwav_file f;
	int error = open_f_index(f, i);
	if (error)
		return error;
	fname.set_ext(".aud");
	if (error = f.process())
		return error;
	const t_riff_wave_format_chunk& format_chunk = f.get_format_chunk();
	int cb_data = f.get_data_header().size;
	int samplerate = format_chunk.samplerate;
	if (format_chunk.tag != 0x0001 ||
		format_chunk.c_channels != 1 && format_chunk.c_channels != 2 ||
		format_chunk.cbits_sample != 16)
		return 0x100;
	Cvirtual_binary data;
	f.seek(f.get_data_ofs());
	if (error = f.read(data.write_start(cb_data), cb_data))
		return error;
	int c_samples = cb_data >> 1;
	if (format_chunk.c_channels == 2)
	{
		audio_combine_channels(reinterpret_cast<__int16*>(data.data_edit()), c_samples);
		c_samples >>= 1;
	}
	if (samplerate == 44100)
	{
		audio_combine_channels(reinterpret_cast<__int16*>(data.data_edit()), c_samples);
		c_samples >>= 1;
		samplerate >>= 1;
	}
	Caud_file_write g;
	if (error = g.open_write(fname))
		return error;
	g.set_c_samples(c_samples);
	g.set_samplerate(samplerate);
	if (error = g.write_header())
		return error;
	aud_decode aud_d;
	aud_d.init();
	const short* r = reinterpret_cast<const short*>(data.data());
	byte chunk[512];
	while (c_samples)
	{
		int cs_chunk = min(c_samples, 1024);
		aud_d.encode_chunk(r, chunk, cs_chunk);
		r += cs_chunk;
		if (error = g.write_chunk(chunk, cs_chunk))
			return error;
		c_samples -= cs_chunk;
	}
	return error;
}

int CXCCMixerView::copy_as_avi(int i, Cfname fname) const
{
	int error = 0;
	fname.set_ext(".avi");
	Cvqa_file f;
	error = open_f_index(f, i);
	if (!error)
		error = f.extract_as_avi(fname, AfxGetMainWnd()->GetSafeHwnd());
	return error;
}

int CXCCMixerView::copy_as_cps(int i, Cfname fname) const
{
	fname.set_ext(".cps");
	Cvirtual_image image = get_vimage(i);
	if (image.cx() != 320 || image.cy() != 200)
		return 257;
	t_palette palette;
	if (image.palette())
	{
		memcpy(palette, image.palette(), sizeof(t_palette));
		convert_palette_24_to_18(palette);
	}
	else
		memcpy(palette, get_default_palette(), sizeof(t_palette));
	if (image.cb_pixel() != 1)
		image.decrease_color_depth(1, palette);
	return cps_file_write(image.image(), palette).save(fname);
}

int CXCCMixerView::copy_as_csv(int i, Cfname fname) const
{
	int error = 0;
	fname.set_ext(".csv");
	Chva_file f;
	error = open_f_index(f, i);
	if (!error)
		error = f.extract_as_csv(fname);
	return error;
}

int CXCCMixerView::copy_as_html(int i, Cfname fname) const
{
	fname.set_ext(".html");
	Ccc_file f(true);
	int error = open_f_index(f, i);
	if (error)
		return error;
	switch (f.get_file_type(false))
	{
	case ft_map_ts:
		{
			Cmap_ts_ini_reader ir;
			ir.fast(true);
			ir.process(f.vdata());
			stringstream ini;
			Cmap_ts_encoder encoder(ini, true);
			Cmap_ts_encoder::t_header header;
			header.cx = ir.get_map_data().size_right;
			header.cy = ir.get_map_data().size_bottom;
			encoder.header(header);
			encoder.process(f.vdata());
			ofstream os(fname);
			ir.write_report(os, fname, encoder);
		}
		break;
	case ft_pkt_ts:
		{
			Cpkt_ts_ini_reader ir;
			ir.process(f.vdata());
			ofstream os(fname);
			ir.write_report(os);
		}
		break;
	}
	return error;
}

int CXCCMixerView::copy_as_hva(int i, Cfname fname) const
{
	fname.set_ext(".hva");
	Ctext_file f;
	int error = open_f_index(f, i);
	return error ? error : hva_file_write(f.get_data(), f.get_size()).save(fname);
}

int CXCCMixerView::copy_as_map_ts_preview(int i, Cfname fname) const
{
	fname.set_ext(".txt");
	Cvirtual_image image = get_vimage(i);
	image.cb_pixel(3);
	Cvirtual_binary d;
	d.set_size(encode5(image.image(), d.write_start(image.cx() * image.cy() * 6), image.cb_image(), 5));
	Cvirtual_binary e = encode64(d);
	ofstream g(fname);
	g << "[Preview]" << endl
		<< "Size=0,0," << image.cx() << ',' << image.cy() << endl
		<< endl
		<< "[PreviewPack]" << endl;
	const byte* r = e.data();
	int line_i = 1;
	while (r < e.data_end())
	{
		char line[80];
		int cb_line = min(e.data_end() - r, 70);
		memcpy(line, r, cb_line);
		line[cb_line] = 0;
		r += cb_line;
		g << line_i++ << '=' << line << endl;
	}
	return g.fail();
}

int CXCCMixerView::copy_as_pal(int i, Cfname fname) const
{
	Cvirtual_image image = get_vimage(i);
	if (!image.palette())
		return 1;
	t_palette palette;
	memcpy(palette, image.palette(), sizeof(t_palette));
	convert_palette_24_to_18(palette);
	fname.set_ext(".pal");
	return file32_write(fname, palette, sizeof(t_palette));
}

int CXCCMixerView::copy_as_pal_jasc(int i, Cfname fname) const
{
	Cpal_file f;
	bool shift_left = false;
	Cvirtual_image image = get_vimage(i);
	if (image.palette())
		f.load(Cvirtual_binary(image.palette(), sizeof(t_palette)));
	else if (open_f_index(f, i))
		return 1;
	else
		shift_left = true;
	fname.set_ext(".pal");
	ofstream os(fname);
	return f.extract_as_pal_jasc(os, shift_left).fail();
}

static int copy_as_image(Cvideo_decoder* v, string fname, t_file_type ft)
{
	t_palette palette;
	memcpy(palette, v->palette(), sizeof(t_palette));
	convert_palette_18_to_24(palette);
	Cvirtual_binary frame;
	Cfname t = fname;
	for (int i = 0; i < v->cf(); i++)
	{
		v->decode(frame.write_start(v->cb_image()));
		t.set_title(Cfname(fname).get_ftitle() + " " + nwzl(4, i));
		if (int error = image_file_write(t, ft, frame.data(), palette, v->cx(), v->cy(), v->cb_pixel()))
			return error;
	}
	delete v;
	return 0;
}

int CXCCMixerView::copy_as_pcx(int i, Cfname fname, t_file_type ft) const
{
	switch (ft)
	{
	case ft_jpeg:
	case ft_jpeg_single:
		fname.set_ext(".jpeg");
		break;
	case ft_pcx:
	case ft_pcx_single:
		fname.set_ext(".pcx");
		break;
	case ft_tga:
	case ft_tga_single:
		fname.set_ext(".tga");
		break;
	default:
		fname.set_ext(".png");
	}
	switch (find_ref(m_index, get_id(i)).ft)
	{
	case ft_cps:
	case ft_dds:
	case ft_jpeg:
	case ft_pcx:
	case ft_png:
	case ft_tga:
	case ft_tmp_ra:
	case ft_tmp_ts:
		return get_vimage(i).save(fname, ft);
	case ft_shp_dune2:
		{
			Cshp_dune2_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_pcx(fname, ft, get_default_palette());
		}
	case ft_shp:
		{
			Cshp_file f;
			int error = open_f_index(f, i);
			return error ? error : copy_as_image(f.decoder(get_default_palette()), fname, ft);
		}
	case ft_shp_ts:
		{
			switch (ft)
			{
			case ft_jpeg_single:
				return get_vimage(i).save(fname, ft_jpeg);
			case ft_pcx_single:
				return get_vimage(i).save(fname, ft_pcx);
			case ft_png_single:
				return get_vimage(i).save(fname, ft_png);
			case ft_tga_single:
				return get_vimage(i).save(fname, ft_tga);
			}
			Cshp_ts_file f;
			f.load(get_vdata(i));
			return f.extract_as_pcx(fname, ft, get_default_palette(), GetMainFrame()->combine_shadows());
		}
	case ft_vqa:
		{
			Cvqa_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_pcx(fname, ft);
		}
	case ft_vxl:
		{
			Cvxl_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_pcx(fname, ft, get_default_palette());
		}
	case ft_wsa_dune2:
		{
			Cwsa_dune2_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_pcx(fname, ft, get_default_palette());
		}
	case ft_wsa:
		{
			Cwsa_file f;
			int error = open_f_index(f, i);
			return error ? error : copy_as_image(f.decoder(), fname, ft);
		}
	}
	return 0;
}

static string get_base_name(const string& fname)
{
	string t = Cfname(fname).get_ftitle();
	int p = -1;
	for (int i = 0; i < t.length(); i++)
	{
		if (!isdigit(t[i]))
			p = i;
	}
	return t.substr(0, p + 1);
}

static int get_index_from_name(const string& base_name, const string& fname)
{
	string t = Cfname(fname).get_ftitle();
	return t.substr(0, base_name.length()) != base_name
		|| base_name.find_first_not_of("0123456789", base_name.length()) != string::npos
		? -1
		: atoi(t.substr(base_name.length()).c_str());
}

static void create_rp(const t_palette s1, const t_palette s2, byte* d, t_game game)
{
	d[0] = 0;
	for (int i = 1; i < 256; i++)
	{
		switch (game)
		{
		case game_td:
			if (i >= 0xb0 && i < 0xc0)
				d[i] = i - 0xa0;
			else
				d[i] = find_color(s1[i].r, s1[i].g, s1[i].b, s2);
			break;
		case game_ra:
			if (i >= 0x50 && i < 0x60)
				d[i] = i - 0x40;
			else
				d[i] = find_color(s1[i].r, s1[i].g, s1[i].b, s2);
			break;
		default:
			d[i] = find_color(s1[i].r, s1[i].g, s1[i].b, s2);
		}
	}
}

int CXCCMixerView::copy_as_shp(int _i, Cfname fname) const
{
	fname.set_ext(".shp");
	Cvirtual_binary s;
	t_palette s_palette;
	string base_name = get_base_name(fname);
	if (get_index_from_name(base_name, fname))
		return em_bad_fname;
	Cvirtual_image image = get_vimage(_i);
	if (image.palette())
		memcpy(s_palette, image.palette(), sizeof(t_palette));
	else
	{
		memcpy(s_palette, get_default_palette(), sizeof(t_palette));
		convert_palette_18_to_24(s_palette);
	}
	int cx = image.cx();
	int cy = image.cy();
	int c_images = 0;
	int index[1000];
	int i;
	for (i = 0; i < 1000; i++)
		index[i] = -1;
	for (auto& j : m_index)
	{
		int z = get_index_from_name(base_name, j.second.name);
		if (z != -1 && z < 1000)
			index[z] = j.first;
	}
	while (i--)
	{
		int id = index[i];
		if (id == -1)
			continue;
		image = get_vimage_id(id);
		if (image.cx() != cx || image.cy() != cy)
			return em_bad_size;
		image.cb_pixel(1, s_palette);
		if (!s.data())
			c_images = i + 1;
		memcpy(s.write_start(cx * cy * c_images) + cx * cy * i, image.image(), cx * cy);
	}
	if (!s.data())
		return 1;
	if (GetMainFrame()->use_palette_for_conversion())
	{
		byte rp[256];
		t_palette p;
		memcpy(p, get_default_palette(), sizeof(t_palette));
		convert_palette_18_to_24(p);
		create_rp(s_palette, p, rp);
		apply_rp(s.data_edit(), cx * cy * c_images, rp);
	}
	trim(base_name);
	fname.set_title(base_name);
	return shp_file_write(s.data(), cx, cy, c_images).save(fname);
}

int CXCCMixerView::copy_as_shp_ts(int i, Cfname fname) const
{
	const bool convert_from_td = GetMainFrame()->convert_from_td();
	const bool convert_from_ra = GetMainFrame()->convert_from_ra();
	const bool convert_shadow = GetMainFrame()->split_shadows();
	int cx;
	int cy;
	int c_images;
	fname.set_ext(".shp");
	Cvirtual_binary s;
	t_palette s_palette;
	string base_name = fname.get_ftitle();
	switch (find_ref(m_index, get_id(i)).ft)
	{
	case ft_shp:
		{
			Cshp_file f;
			int error = open_f_index(f, i);
			if (error)
				return error;
			memcpy(s_palette, GetMainFrame()->get_game_palette(convert_from_td ? game_td : game_ra), sizeof(t_palette));
			convert_palette_18_to_24(s_palette);
			cx = f.cx();
			cy = f.cy();
			c_images = f.cf() << 1;
			shp_images::t_image_data* p;
			byte* w = s.write_start(cx * cy * c_images);
			if (cx && cy && !shp_images::load_shp(f, p))
			{
				for (int i = 0; i < c_images >> 1; i++)
				{
					memcpy(w, p->get(i), cx * cy);
					w += cx * cy;
				}
				shp_images::destroy_shp(p);
			}
			break;
		}
	default:
		base_name = get_base_name(fname);
		if (get_index_from_name(base_name, fname))
			return em_bad_fname;
		Cvirtual_image image = get_vimage(i);
		if (image.palette())
			memcpy(s_palette, image.palette(), sizeof(t_palette));
		else
		{
			memcpy(s_palette, get_default_palette(), sizeof(t_palette));
			convert_palette_18_to_24(s_palette);
		}
		cx = image.cx();
		cy = image.cy();
		c_images = 0;
		int index[10000];
		int i;
		for (i = 0; i < 10000; i++)
			index[i] = -1;
		for (auto& j : m_index)
		{
			int z = get_index_from_name(base_name, j.second.name);
			if (z != -1 && z < 10000)
				index[z] = j.first;
		}
		while (i--)
		{
			int id = index[i];
			if (id == -1)
				continue;
			image = get_vimage_id(id);
			if (image.cx() != cx || image.cy() != cy)
				return em_bad_size;
			image.cb_pixel(1, s_palette);
			if (!s.data())
			{
				c_images = i + 1;
				if (convert_shadow)
					c_images <<= 1;
			}
			memcpy(s.write_start(cx * cy * c_images) + cx * cy * i, image.image(), cx * cy);
		}
		break;
	}
	if (!s.data())
		return 1;
	if (convert_shadow)
	{
		int count = cx * cy * c_images >> 1;
		byte* r = s.data_edit();
		byte* w = s.data_edit() + count;
		while (count--)
		{
			byte& v = *r++;
			if (v == 4)
			{
				v = 0;
				*w++ = 1;
			}
			else
				*w++ = 0;
		}
	}
	if (GetMainFrame()->use_palette_for_conversion())
	{
		byte rp[256];
		t_palette p;
		memcpy(p, get_default_palette(), sizeof(t_palette));
		convert_palette_18_to_24(p);
		if (convert_from_td)
			create_rp(s_palette, p, rp, game_td);
		else if (convert_from_ra)
			create_rp(s_palette, p, rp, game_ra);
		else
			create_rp(s_palette, p, rp);
		apply_rp(s.data_edit(), cx * cy * c_images >> convert_shadow, rp);
	}
	if (GetMainFrame()->fix_shadows() && ~c_images & 1)
	{
		int count = cx * cy * c_images >> 1;
		for (byte* w = s.data_edit() + count; count--; w++)
		{
			if (*w)
				*w = 1;
		}
	}
	trim(base_name);
	fname.set_title(base_name);
	return shp_ts_file_write(s.data(), cx, cy, c_images, GetMainFrame()->enable_compression()).save(fname);
}

int CXCCMixerView::copy_as_text(int i, Cfname fname) const
{
	fname.set_ext(".txt");
	switch (find_ref(m_index, get_id(i)).ft)
	{
	case ft_st:
		{
			Cst_file f;
			if (int error = open_f_index(f, i))
				return error;
			ofstream os(fname);
			return f.extract_as_text(os).fail();
		}
	case ft_vxl:
		{
			Cvxl_file f;
			if (int error = open_f_index(f, i))
				return error;
			ofstream os(fname);
			return f.extract_as_text(os).fail();
		}
	case ft_xif:
		{
			Cxif_file f;
			if (int error = open_f_index(f, i))
				return error;
			Cxif_key key;
			if (int error = f.decode(key))
				return error;
			ofstream os(fname);
			key.dump(os, true);
			return 0;
		}
	case ft_pal:
		{
			Cpal_file f;
			if (int error = open_f_index(f, i))
				return error;
			ofstream os(fname);
			return f.extract_as_text(os).fail();
		}
	case ft_mix:
		{
			Cmix_file f;
			if (int error = open_f_index(f, i))
				return error;
			ofstream os(fname);
			return f.extract_as_text(os).fail();
		}
	}
	return 0;
}

int CXCCMixerView::copy_as_vxl(int i, Cfname fname) const
{
	int error = 0;
	fname.set_ext(".vxl");
	switch (find_ref(m_index, get_id(i)).ft)
	{
	case ft_text:
		{
			Cvirtual_tfile f;
			f.load_data(get_vdata(i));
			Cvirtual_binary d = vxl_file_write(f);
			return d.size() ? d.save(fname) : 1;
		}
	case ft_xif:
		{
			Cxif_key k;
			int error = k.load_key(get_vdata(i));
			if (error)
				return error;
			Cvirtual_binary d = vxl_file_write(k);
			return d.size() ? d.save(fname) : 1;
		}
	default:
		{
			string base_name = get_base_name(fname);
			if (get_index_from_name(base_name, fname))
				return em_bad_fname;
			Cvirtual_image image = get_vimage(i);
			int cx = image.cx();
			int cy = image.cy();
			Cvirtual_binary s;
			int index[256];
			int i;
			for (i = 0; i < 256; i++)
				index[i] = -1;
			for (auto& j : m_index)
			{
				int z = get_index_from_name(base_name, j.second.name);
				if (z != -1 && z < 256)
					index[z] = j.first;
			}
			int c_images = 0;
			while (i--)
			{
				int id = index[i];
				if (id == -1)
					continue;
				image = get_vimage_id(id);
				if (image.cx() != cx || image.cy() != cy)
					return em_bad_size;
				if (image.cb_pixel() != 1)
					return em_bad_depth;
				if (!s.data())
					c_images = i + 1;
				memcpy(s.write_start(cx * cy * c_images) + cx * cy * i, image.image(), cx * cy);
			}
			Cvirtual_binary d = vxl_file_write(s.data(), NULL, cx, cy, c_images);
			trim(base_name);
			fname.set_title(base_name);
			return d.size() ? d.save(fname) : 1;
		}
	}
	return error;
}

int CXCCMixerView::copy_as_wav_ima_adpcm(int i, Cfname fname) const
{
	fname.set_ext(".wav");
	switch (find_ref(m_index, get_id(i)).ft)
	{
	case ft_wav:
		{
			Cwav_file f;
			int error = open_f_index(f, i);
			if (error)
				return error;
			if (error = f.process())
				return error;
			const t_riff_wave_format_chunk& format_chunk = f.get_format_chunk();
			if (format_chunk.tag != 1
				|| format_chunk.c_channels != 1 && format_chunk.c_channels != 2
				|| format_chunk.cbits_sample != 16)
				return 0x100;
			int cb_s = f.get_data_header().size;
			Cvirtual_binary s;
			f.seek(f.get_data_ofs());
			if (error = f.read(s.write_start(cb_s), cb_s))
				return error;
			int c_channels = format_chunk.c_channels;
			Cima_adpcm_wav_encode encode;
			encode.load(reinterpret_cast<short*>(s.data_edit()), cb_s, c_channels);
			int c_samples = encode.c_samples();
			int cb_audio = encode.cb_data();
			int cb_d = sizeof(t_wav_ima_adpcm_header) + cb_audio;
			Cvirtual_binary d;
			byte* w = d.write_start(cb_d);
			w += wav_ima_adpcm_file_write_header(w, cb_audio, c_samples, format_chunk.samplerate, c_channels);
			memcpy(w, encode.data(), cb_audio);
			return d.save(fname);
		}
	}
	return 0;
}

int CXCCMixerView::copy_as_wav_pcm(int i, Cfname fname) const
{
	fname.set_ext(".wav");
	switch (find_ref(m_index, get_id(i)).ft)
	{
	case ft_aud:
		{
			Caud_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_wav(fname);
		}
	case ft_voc:
		{
			Cvoc_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_wav(fname);
		}
	case ft_vqa:
		{
			Cvqa_file f;
			int error = open_f_index(f, i);
			return error ? error : f.extract_as_wav(fname);
		}
	case ft_wav:
		{
			Cwav_file f;
			int error = open_f_index(f, i);
			if (error)
				return error;
			if (error = f.process())
				return error;
			const t_riff_wave_format_chunk& format_chunk = f.get_format_chunk();
			if (format_chunk.tag != 0x11
				|| format_chunk.c_channels != 1 && format_chunk.c_channels != 2
				|| format_chunk.cbits_sample != 4)
				return 0x100;
			int cb_s = f.get_data_header().size;
			Cvirtual_binary s;
			f.seek(f.get_data_ofs());
			if (error = f.read(s.write_start(cb_s), cb_s))
				return error;
			int c_channels = format_chunk.c_channels;
			int c_samples = f.get_fact_chunk().c_samples;
			int cb_audio = c_channels * c_samples << 1;
			int cb_d = sizeof(t_wav_header) + cb_audio;
			Cvirtual_binary d;
			byte* w = d.write_start(cb_d);
			w += wav_file_write_header(w, c_samples, format_chunk.samplerate, 2, c_channels);
			Cima_adpcm_wav_decode decode;
			decode.load(s.data(), cb_s, c_channels, 512 * c_channels);
			memcpy(w, decode.data(), cb_audio);
			return d.save(fname);
		}
	}
	return 0;
}

int CXCCMixerView::copy_as_xif(int i, Cfname fname) const
{
	fname.set_ext(".xif");
	Cvxl_file f;
	int error = open_f_index(f, i);
	return error ? error : f.extract_as_xif(fname);
}

void CXCCMixerView::OnPopupCopy()
{
	copy_as(static_cast<t_file_type>(-1));
}

void CXCCMixerView::OnUpdatePopupCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy());
}

void CXCCMixerView::OnPopupCopyAsAUD()
{
	copy_as(ft_aud);
}

void CXCCMixerView::OnUpdatePopupCopyAsAUD(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_aud));
}

void CXCCMixerView::OnPopupCopyAsAVI()
{
	copy_as(ft_avi);
}

void CXCCMixerView::OnUpdatePopupCopyAsAVI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_avi));
}

void CXCCMixerView::OnPopupCopyAsCPS()
{
	copy_as(ft_cps);
}

void CXCCMixerView::OnUpdatePopupCopyAsCPS(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_cps));
}

void CXCCMixerView::OnPopupCopyAsCSV()
{
	copy_as(ft_csv);
}

void CXCCMixerView::OnUpdatePopupCopyAsCSV(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_csv));
}

void CXCCMixerView::OnPopupCopyAsHTML()
{
	copy_as(ft_html);
}

void CXCCMixerView::OnUpdatePopupCopyAsHTML(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_html));
}

void CXCCMixerView::OnPopupCopyAsHVA()
{
	copy_as(ft_hva);
}

void CXCCMixerView::OnUpdatePopupCopyAsHVA(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_hva));
}

void CXCCMixerView::OnPopupCopyAsJpegSingle()
{
	copy_as(ft_jpeg_single);
}

void CXCCMixerView::OnUpdatePopupCopyAsJpegSingle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_jpeg_single));
}

void CXCCMixerView::OnPopupCopyAsJpeg()
{
	copy_as(ft_jpeg);
}

void CXCCMixerView::OnUpdatePopupCopyAsJpeg(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_jpeg));
}

void CXCCMixerView::OnPopupCopyAsMapTsPreview()
{
	copy_as(ft_map_ts_preview);
}

void CXCCMixerView::OnUpdatePopupCopyAsMapTsPreview(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_map_ts_preview));
}

void CXCCMixerView::OnPopupCopyAsPAL()
{
	copy_as(ft_pal);
}

void CXCCMixerView::OnUpdatePopupCopyAsPAL(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_pal));
}

void CXCCMixerView::OnPopupCopyAsPAL_JASC()
{
	copy_as(ft_pal_jasc);
}

void CXCCMixerView::OnUpdatePopupCopyAsPAL_JASC(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_pal_jasc));
}

void CXCCMixerView::OnPopupCopyAsPCX()
{
	copy_as(ft_pcx);
}

void CXCCMixerView::OnUpdatePopupCopyAsPCX(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_pcx));
}

void CXCCMixerView::OnPopupCopyAsPcxSingle()
{
	copy_as(ft_pcx_single);
}

void CXCCMixerView::OnUpdatePopupCopyAsPcxSingle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_pcx_single));
}

void CXCCMixerView::OnPopupCopyAsPngSingle()
{
	copy_as(ft_png_single);
}

void CXCCMixerView::OnUpdatePopupCopyAsPngSingle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_png_single));
}

void CXCCMixerView::OnPopupCopyAsPNG()
{
	copy_as(ft_png);
}

void CXCCMixerView::OnUpdatePopupCopyAsPNG(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_png));
}

void CXCCMixerView::OnPopupCopyAsSHP()
{
	copy_as(ft_shp);
}

void CXCCMixerView::OnUpdatePopupCopyAsSHP(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_shp));
}

void CXCCMixerView::OnPopupCopyAsSHP_TS()
{
	copy_as(ft_shp_ts);
}

void CXCCMixerView::OnUpdatePopupCopyAsSHP_TS(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_shp_ts));
}


void CXCCMixerView::OnPopupCopyAsTgaSingle()
{
	copy_as(ft_tga_single);
}

void CXCCMixerView::OnUpdatePopupCopyAsTgaSingle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_tga_single));
}

void CXCCMixerView::OnPopupCopyAsTga()
{
	copy_as(ft_tga);
}

void CXCCMixerView::OnUpdatePopupCopyAsTga(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_tga));
}

void CXCCMixerView::OnPopupCopyAsText()
{
	copy_as(ft_text);
}

void CXCCMixerView::OnUpdatePopupCopyAsText(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_text));
}

void CXCCMixerView::OnPopupCopyAsWavImaAdpcm()
{
	copy_as(ft_wav_ima_adpcm);
}

void CXCCMixerView::OnUpdatePopupCopyAsWavImaAdpcm(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_wav_ima_adpcm));
}

void CXCCMixerView::OnPopupCopyAsWavPcm()
{
	copy_as(ft_wav_pcm);
}

void CXCCMixerView::OnUpdatePopupCopyAsWavPcm(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_wav_pcm));
}

void CXCCMixerView::OnPopupCopyAsWSA()
{
	copy_as(ft_wsa);
}

void CXCCMixerView::OnUpdatePopupCopyAsWSA(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_wsa));
}

void CXCCMixerView::OnPopupCopyAsVXL()
{
	copy_as(ft_vxl);
}

void CXCCMixerView::OnUpdatePopupCopyAsVXL(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_vxl));
}

void CXCCMixerView::OnPopupCopyAsXIF()
{
	copy_as(ft_xif);
}

void CXCCMixerView::OnUpdatePopupCopyAsXIF(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_copy_as(ft_xif));
}

void CXCCMixerView::OnDropFiles(HDROP hDropInfo)
{
	CWaitCursor wait;
	int error = 0;
	int c_files = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	char fname[MAX_PATH];
	if (m_mix_f)
	{
		t_file_type ft = m_mix_f->get_file_type();
		close_location(false);
		switch (ft)
		{
		case ft_big:
			{
				Cbig_edit f;
				error = f.open(m_mix_fname);
				if (!error)
				{
					for (int i = 0; i < c_files; i++)
					{
						DragQueryFile(hDropInfo, i, fname, MAX_PATH);
						DWORD file_attributes = GetFileAttributes(fname);
						if (file_attributes == INVALID_FILE_ATTRIBUTES || ~file_attributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							Cvirtual_binary d;
							if (!d.load(fname))
								error = error ? f.insert(Cfname(fname).get_fname(), d) : f.insert(Cfname(fname).get_fname(), d), error;
						}
						else
							error = error ? big_insert_dir(f, string(fname) + '/', Cfname(fname).get_fname() + '\\'), error : big_insert_dir(f, string(fname) + '/', Cfname(fname).get_fname() + '\\');
					}
					error = error ? f.write_index(), error : f.write_index();
					f.close();
				}
				break;
			}
		case ft_mix_rg:
			{
				Cmix_rg_edit f;
				error = f.open(m_mix_fname);
				if (!error)
				{
					for (int i = 0; i < c_files; i++)
					{
						DragQueryFile(hDropInfo, i, fname, MAX_PATH);
						Cvirtual_binary d;
						if (!d.load(fname))
							error = error ? f.insert(Cfname(fname).get_fname(), d) : f.insert(Cfname(fname).get_fname(), d), error;
					}
					error = error ? f.write_index(), error : f.write_index();
					f.close();
				}
				break;
			}
		default:
			Cmix_edit f;
			error = f.open(m_mix_fname);
			if (!error)
			{
				for (int i = 0; i < c_files; i++)
				{
					DragQueryFile(hDropInfo, i, fname, MAX_PATH);
					Cvirtual_binary d;
					if (!d.load(fname))
						error = error ? f.insert(Cfname(fname).get_fname(), d) : f.insert(Cfname(fname).get_fname(), d), error;
				}
				error = error ? f.write_index(), error : f.write_index();
				f.close();
			}
		}
		open_location_mix(m_mix_fname);
		OnPopupCompact();
	}
	else
	{
		for (int i = 0; i < c_files; i++)
		{
			DragQueryFile(hDropInfo, i, fname, MAX_PATH);
			error = copy_file(fname, get_dir() + Cfname(fname).get_fname());
		}
		update_list();
	}
	set_msg(error ? "Insert failed" : "Insert done");
	DragFinish(hDropInfo);
}

void CXCCMixerView::OnPopupCompact()
{
	CWaitCursor wait;
	int error = 0;
	t_file_type ft = m_mix_f->get_file_type();
	close_location(false);
	switch (ft)
	{
	case ft_big:
		{
			Cbig_edit f;
			error = f.open(m_mix_fname);
			if (!error)
			{
				error = f.compact();
				f.close();
			}
			break;
		}
	case ft_mix_rg:
		{
			Cmix_rg_edit f;
			error = f.open(m_mix_fname);
			if (!error)
			{
				error = f.compact();
				f.close();
			}
			break;
		}
	default:
		Cmix_edit f;
		error = f.open(m_mix_fname);
		if (!error)
		{
			error = f.compact();
			f.close();
		}
	}
	set_msg(error ? "Compact failed" : "Compact done");
	open_location_mix(m_mix_fname);
}

void CXCCMixerView::OnUpdatePopupCompact(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_location.size() == 1);
}

bool DeleteDirectory(string strPath, const int uiEndLevel = -1, int iCurrentLevel = 0)
{
	if (iCurrentLevel > uiEndLevel)
	{
		return true;
	}
	WIN32_FIND_DATA findFileData;
	auto strFindPath = strPath + "\\*";

	HANDLE hFind = FindFirstFile(strFindPath.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE == hFind) {
		return false;
	}

	bool result = true;
	do
	{
		string strFileName = findFileData.cFileName;
		if (strFileName != "." && strFileName != "..")
		{
			strFindPath = strPath + "\\" + strFileName;
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!DeleteDirectory(strFindPath, uiEndLevel, iCurrentLevel + 1) || !RemoveDirectory(strFindPath.c_str()))
				{
					result = false;
					break;
				}
			}
			else
			{
				if (!DeleteFile(strFindPath.c_str()))
				{
					result = false;
					break;
				}
			}
		}
	} while (FindNextFile(hFind, &findFileData));
	FindClose(hFind);
	if (result && !RemoveDirectory(strPath.c_str()))
	{
		result = false;
	}
	return result;
}

void CXCCMixerView::OnPopupDelete()
{
	if (~GetAsyncKeyState(VK_SHIFT) < 0 && MessageBox("Are you sure you want to delete these files?", NULL, MB_ICONQUESTION | MB_YESNO) != IDYES)
		return;
	CWaitCursor wait;
	int error = 0;
	if (m_mix_f)
	{
		t_file_type ft = m_mix_f->get_file_type();
		close_location(false);
		switch (ft)
		{
		case ft_big:
		{
			Cbig_edit f;
			error = f.open(m_mix_fname);
			if (!error)
			{
				for (auto& i : m_index_selected)
					f.erase(find_ref(m_index, get_id(i)).name);
				error = f.write_index();
				f.close();
			}
			break;
		}
		case ft_mix_rg:
		{
			Cmix_rg_edit f;
			error = f.open(m_mix_fname);
			if (!error)
			{
				for (auto& i : m_index_selected)
					f.erase(find_ref(m_index, get_id(i)).name);
				error = f.write_index();
				f.close();
			}
			break;
		}
		default:
			Cmix_edit f;
			error = f.open(m_mix_fname);
			if (!error)
			{
				for (auto& i : m_index_selected)
					f.erase(get_id(i));
				error = f.write_index();
				f.close();
			}
		}
		open_location_mix(m_mix_fname);
		OnPopupCompact();
	}
	else
	{
		for (auto& i : m_index_selected)
		{
			const t_index_entry& index = find_ref(m_index, get_id(i));
			if (index.ft == ft_dir)
			{
				if (!DeleteDirectory(m_dir + index.name, 1))
					error = 1;
			}
			else if (!DeleteFile((m_dir + index.name).c_str()))
				error = 1;
		}
	}
	set_msg(error ? "Delete failed" : "Delete done");
	update_list();
}

void CXCCMixerView::OnUpdatePopupDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_edit() && can_delete());
}

t_game CXCCMixerView::get_game()
{
	t_game r = GetMainFrame()->get_game();
	return r == -1 ? m_game : r;
}

void CXCCMixerView::OnPopupOpen()
{
	open_item(get_current_id());
}

void CXCCMixerView::OnUpdatePopupOpen(CCmdUI* pCmdUI)
{
	int id = get_current_id();
	if (id != -1)
	{
		switch (find_ref(m_index, id).ft)
		{
		case ft_aud:
		case ft_ogg:
		case ft_voc:
		case ft_wav:
			pCmdUI->Enable(!!GetMainFrame()->get_ds());
			return;
		case ft_big:
		case ft_dir:
		case ft_drive:
		case ft_lnkdir:
		case ft_mix:
		case ft_mix_rg:
		case ft_pak:
		case ft_shp_ts:
		case ft_vqa:
		case ft_wsa:
			pCmdUI->Enable(true);
			return;
		}
	}
	pCmdUI->Enable(false);
}

void CXCCMixerView::OnPopupExplore()
{
	ShellExecute(m_hWnd, "open", m_dir.c_str(), NULL, NULL, SW_SHOW);
}

void CXCCMixerView::OnUpdatePopupExplore(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_mix_f);
}

void CXCCMixerView::OnPopupRefresh()
{
	update_list();
}

void CXCCMixerView::OnUpdatePopupRefresh(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_accept());
}

int CXCCMixerView::resize(int id)
{
	Cfname fname = get_dir() + find_ref(m_index, id).name;
	switch (find_ref(m_index, id).ft)
	{
	case ft_jpeg:
	case ft_pcx:
	case ft_png:
	case ft_tga:
		{
			Cvirtual_image s = get_vimage_id(id);
			CResizeDlg dlg;
			dlg.set_size(s.cx(), s.cy());
			if (IDOK != dlg.DoModal())
				return 1;
			int cb_pixel = s.cb_pixel();
			Cvirtual_image s_palette = s;
			s.cb_pixel(4);
			Cvirtual_image d(NULL, dlg.get_cx(), dlg.get_cy(), 4, s_palette.palette());
			if (s.cx() < d.cx())
				resize_image_up(reinterpret_cast<const t_palette32_entry*>(s.image()), reinterpret_cast<t_palette32_entry*>(d.image_edit()), s.cx(), s.cy(), d.cx(), d.cy());
			else
				resize_image_down(reinterpret_cast<const t_palette32_entry*>(s.image()), reinterpret_cast<t_palette32_entry*>(d.image_edit()), s.cx(), s.cy(), d.cx(), d.cy());
			d.cb_pixel(cb_pixel, s_palette.palette());
			return d.save(fname, find_ref(m_index, id).ft);
		}
	case ft_shp_ts:
		{
			Cshp_ts_file f;
			int error = open_f_index(f, get_current_index());
			if (error)
				return error;
			const int global_cx = f.cx();
			const int global_cy = f.cy();
			CResizeDlg dlg;
			dlg.set_size(global_cx, global_cy);
			if (IDOK != dlg.DoModal())
				return 1;
			const int global_cx_d = dlg.get_cx();
			const int global_cy_d = dlg.get_cy();
			const int c_images = f.cf();
			t_palette palette;
			convert_palette_18_to_24(get_default_palette(), palette);
			palette[0].r = palette[0].b = 0xff;
			palette[0].g = 0;
			Cvirtual_binary rp;
			if (global_cx_d * global_cy_d * c_images > 1 << 18)
				create_downsample_table(palette, rp.write_start(1 << 18));
			Cvirtual_binary d8(NULL, global_cx_d * global_cy_d * c_images);
			t_palette32_entry* d32 = new t_palette32_entry[global_cx_d * global_cy_d];
			Cvirtual_binary image8(NULL, global_cx * global_cy);
			t_palette32_entry* image32 = new t_palette32_entry[global_cx * global_cy];
			for (int i = 0; i < c_images; i++)
			{
				set_msg("Resize: " + n(i * 100 / c_images) + "%");
				const int cx = f.get_cx(i);
				const int cy = f.get_cy(i);
				byte* image = new byte[cx * cy];
				const byte* r;
				if (f.is_compressed(i))
				{
					RLEZeroTSDecompress(f.get_image(i), image, cx, cy);
					r = image;
				}
				else
					r = f.get_image(i);
				memset(image8.data_edit(), 0, global_cx * global_cy);
				byte* w = image8.data_edit() + f.get_x(i) + global_cx * f.get_y(i);
				for (int y = 0; y < cy; y++)
				{
					memcpy(w, r, cx);
					r += cx;
					w += global_cx;
				}
				delete[] image;
				if (global_cx == global_cx_d && global_cy == global_cy_d)
					memcpy(d8.data_edit() + global_cx_d * global_cy_d * i, image8.data(), global_cx * global_cy);
				else
				{
					upsample_image(image8.data(), image32, global_cx, global_cy, palette);
					if (global_cx < global_cx_d)
						resize_image_up(image32, d32, global_cx, global_cy, global_cx_d, global_cy_d);
					else
						resize_image_down(image32, d32, global_cx, global_cy, global_cx_d, global_cy_d);
					if (rp.size())
						downsample_image(d32, d8.data_edit() + global_cx_d * global_cy_d * i, global_cx_d, global_cy_d, rp.data());
					else
						downsample_image(d32, d8.data_edit() + global_cx_d * global_cy_d * i, global_cx_d, global_cy_d, palette);
				}
			}
			if (dlg.m_fix_shadows && ~c_images & 1)
			{
				int count = global_cx_d * global_cy_d * c_images >> 1;
				for (byte* w = d8.data_edit() + count; count--; w++)
				{
					if (*w)
						*w = 1;
				}
			}
			delete[] image32;
			delete[] d32;
			f.close();
			return shp_ts_file_write(d8.data(), global_cx_d, global_cy_d, c_images).save(fname);
		}
	}
	return 1;
}

void CXCCMixerView::OnPopupResize()
{
	int id = get_current_id();
	Cfname fname = get_dir() + find_ref(m_index, id).name;
	int error = resize(id);
	set_msg("Resize " + fname.get_ftitle() + (error ? " failed, error " + n(error) : " succeeded"));
	update_list();
}

void CXCCMixerView::OnUpdatePopupResize(CCmdUI* pCmdUI)
{
	int id = get_current_id();
	if (can_accept() && id != -1)
	{
		switch (find_ref(m_index, id).ft)
		{
		case ft_jpeg:
		case ft_pcx:
		case ft_png:
		case ft_shp_ts:
		case ft_tga:
			pCmdUI->Enable(true);
			return;
		}
	}
	pCmdUI->Enable(false);
}

void CXCCMixerView::OnPopupClipboardCopy()
{
	get_vimage(get_current_index()).set_clipboard();
}

void CXCCMixerView::OnUpdatePopupClipboardCopy(CCmdUI* pCmdUI)
{
	int id = get_current_id();
	pCmdUI->Enable(id != -1 && can_convert(find_ref(m_index, id).ft, ft_clipboard));
}

int CXCCMixerView::get_paste_fname(string& fname, t_file_type ft, const char* extension, const char* filter)
{
	int id = get_current_id();
	bool replace = id != -1 && find_ref(m_index, id).ft == ft;
	CFileDialog dlg(false, extension, replace ? (m_dir + find_ref(m_index, id).name).c_str() : NULL, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, filter, this);
	if (!replace)
		dlg.m_ofn.lpstrInitialDir = m_dir.c_str();
	if (IDOK != dlg.DoModal())
		return 1;
	fname = dlg.GetPathName();
	return 0;
}

void CXCCMixerView::paste_as_image(t_file_type ft, const char* extension, const char* filter)
{
	Cvirtual_image image;
	int error = image.get_clipboard();
	if (!error)
	{
		string fname;
		if (get_paste_fname(fname, ft, extension, filter))
			return;
		image.save(fname, ft);
		update_list();
	}
}

void CXCCMixerView::OnPopupClipboardPasteAsJpeg()
{
	paste_as_image(ft_jpeg, "jpeg", "JPEG files (*.jpeg;*.jpg)|*.jpeg;*.jpg|");
}

void CXCCMixerView::OnPopupClipboardPasteAsPcx()
{
	paste_as_image(ft_pcx, "pcx", "PCX files (*.pcx)|*.pcx|");
}

void CXCCMixerView::OnPopupClipboardPasteAsPng()
{
	paste_as_image(ft_png, "png", "PNG files (*.png)|*.png|");
}

void CXCCMixerView::OnPopupClipboardPasteAsTga()
{
	paste_as_image(ft_tga, "tga", "TGA files (*.tga)|*.tga|");
}

void CXCCMixerView::OnUpdatePopupClipboardPasteAsImage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_accept() && IsClipboardFormatAvailable(CF_DIB));
}

void CXCCMixerView::OnPopupClipboardPasteAsShpTs()
{
	Cvirtual_image image;
	int error = image.get_clipboard();
	if (!error)
	{
		if (image.cb_pixel() == 3 || GetMainFrame()->use_palette_for_conversion())
		{
			t_palette p;
			memcpy(p, get_default_palette(), sizeof(t_palette));
			convert_palette_18_to_24(p);
			if (image.cb_pixel() == 3)
				image.decrease_color_depth(1, p);
			else
			{
				byte rp[256];
				if (GetMainFrame()->convert_from_td())
					create_rp(image.palette(), p, rp, game_td);
				else if (GetMainFrame()->convert_from_ra())
					create_rp(image.palette(), p, rp, game_ra);
				else
					create_rp(image.palette(), p, rp);
				apply_rp(image.image_edit(), image.cx() * image.cy(), rp);
			}
		}
		string fname;
		if (!get_paste_fname(fname, ft_shp_ts, "shp", "SHP files (*.shp)|*.shp|"))
		{
			Cshp_properties_dlg dlg;
			Cshp_ts_file f;
			int split_shadows = GetMainFrame()->split_shadows();
			int id = get_current_id();
			if (id != -1 && !open_f_id(f, id))
			{
				if (f.is_valid())
				{
					int c_images = f.cf();
					if (c_images < 2)
						split_shadows = false;
					dlg.set_size(f.cx(), f.cy(), c_images >> split_shadows);
				}
				else
					dlg.set_size(image.cx(), image.cy(), 1);
				f.close();
			}
			else
				dlg.set_size(image.cx(), image.cy(), 1);
			if (IDOK == dlg.DoModal())
			{
				int cblocks_x = image.cx() / dlg.get_cx();
				int cblocks_y = image.cy() / dlg.get_cy();
				int c_blocks = dlg.get_c_frames();
				shp_split_frames(image, cblocks_x, cblocks_y);
				if (split_shadows)
				{
					shp_split_shadows(image);
					c_blocks <<= 1;
				}
				shp_ts_file_write(image.image(), dlg.get_cx(), dlg.get_cy(), c_blocks, GetMainFrame()->enable_compression()).save(fname);
				update_list();
			}
		}
	}
}

void CXCCMixerView::OnUpdatePopupClipboardPasteAsVideo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(can_accept() && IsClipboardFormatAvailable(CF_DIB));
}

BOOL CXCCMixerView::OnIdle(LONG lCount)
{
	if (m_reading)
	{
		for (auto& i : m_index)
		{
			t_index_entry& e = i.second;
			if (e.ft != -1)
				continue;
			Ccc_file f(false);
			if (f.open(m_dir + e.name))
			{
				e.ft = ft_unknown;
				e.size = -1;
			}
			else
			{
				e.ft = f.get_file_type();
				e.size = totalSize(f.get_size());
			}

			if (e.ft == ft_unknown)
			{
				Cfname fname = to_lower(e.name);
				if (fname.get_fext() == ".mix")
				{
					//e.ft = f.get_file_type_ext();
					e.ft = ft_mix;
					e.size = totalSize(f.get_size());
				}
			}
			CListCtrl& lc = GetListCtrl();
			LVFINDINFO lvf;
			lvf.flags = LVFI_PARAM;
			lvf.lParam = i.first;
			lc.Update(lc.FindItem(&lvf, -1));
			m_sort_column = -1;
			return true;
		}
		sort_list(0, false);
		sort_list(1, false);
		autosize_colums();
		m_reading = false;
	}
	return false;
}

string CXCCMixerView::report() const
{
	string page;
	ULARGE_INTEGER available, total, free;
	if (GetDiskFreeSpaceEx(m_dir.c_str(), &available, &total, &free))
		page += "<tr><td>" + m_dir + "<td align=right>" + n(available.QuadPart) + "<td align=right>" + n(total.QuadPart) + "<td>&nbsp;";
	page += "<tr><th>Name<th>Type<th>Size<th>Description";
	CListCtrl& lc = GetListCtrl();
	for (int i = 0; i < lc.GetItemCount(); i++)
	{
		const t_index_entry& e = find_ref(m_index, lc.GetItemData(i));
		page += "<tr><td>" + e.name + "<td>" + ft_name[e.ft] + "<td align=right>" + (e.size == "" ? "&nbsp;" : e.size) + "<td>" + (e.description.empty() ? "&nbsp;" : e.description);
	}
	return "<table border=1 width=100%>" + page + "</table>";
}

void CXCCMixerView::extract_open_audio_pak(const string& bag, const string& idx) const
{
	string bag_path = m_dir + bag;
	string idx_path = m_dir + idx;
	if (m_mix_f && (!Cfname(bag_path).exists() || !Cfname(idx_path).exists()))
	{
		int idx_id, bag_id;
		idx_id = Cmix_file::get_id(game_ra2_yr, idx);
		bag_id = Cmix_file::get_id(game_ra2_yr, bag);
		Ccc_file f_bag(false);
		if (!open_f_id(f_bag, bag_id))
			f_bag.extract(bag_path);
		Ccc_file f_idx(false);
		if (!open_f_id(f_idx, idx_id))
			f_idx.extract(idx_path);
	}

	CXSE_dlg dlg(game_ra2_yr);
	dlg.bag_file(bag_path);
	dlg.idx_file(idx_path);
	dlg.DoModal();
}

void CXCCMixerView::open_item(int id)
{
	const t_index_entry& index = find_ref(m_index, id);
	switch (index.ft)
	{
	case ft_aud:
	case ft_ogg:
	case ft_voc:
	case ft_wav:
	{
		if (GetMainFrame()->get_ds())
		{
			CWaitCursor wait;
			Ccc_file f(true);
			if (!open_f_id(f, id))
			{
				xap_play(GetMainFrame()->get_ds(), f.vdata(), index.name);
			}
		}
		break;
	}
	case ft_dir:
		{
			string name = index.name;
			if (name == "..")
			{
				if (!m_mix_f)
				{
					close_location(false);
					int i = m_dir.rfind('\\');
					if (i != string::npos)
					{
						i = m_dir.rfind('\\', i - 1);
						if (i != string::npos)
							open_location_dir(m_dir.substr(0, i + 1));
					}
				}
				else
				{
					close_location(false);
					open_location_dir(m_dir + '\\');
				}
			}
			else
			{
				close_location(false);
				open_location_dir(m_dir + name + '\\');
			}
			break;
		}
	case ft_lnkdir:
	{
		close_location(false);
		Cfname lnkdir(index.name);
		CString cs;
		resolveShortcutTarget(m_hWnd, (m_dir + lnkdir.get_fname()).c_str(), cs);
		m_dir = cs + '\\';
		update_list();
		break;
	}
	case ft_drive:
		{
			string name = index.name;
			if (name == "Browse...")
			{
				CFolderPickerDialog dlg(m_dir.c_str(), NULL, this);
				if (IDOK == dlg.DoModal())
				{
					close_all_locations();
					open_location_dir(static_cast<string>(dlg.GetPathName() + '\\'));
				}
				break;
			}
			close_location(false);
			open_location_dir(name);
			break;
		}
	case ft_big:
	case ft_mix:
	case ft_mix_rg:
	case ft_pak:
		{
			if (m_mix_f)
				open_location_mix(id);
			else
				open_location_mix(m_dir + index.name);
			break;
		}
	case ft_csf:
	{
		CXSTE_dlg dlg2(game_unknown);
		dlg2.open(m_dir + index.name);
		dlg2.DoModal();
		break;
	}
	case ft_unknown:
		{
			Cfname unknown_file(index.name);
			t_index_entry* ptr_entry;
			if (unknown_file.get_fext() == ".bag")
			{
				extract_open_audio_pak(unknown_file, unknown_file.get_ftitle() + ".idx");
			}
			else if (unknown_file.get_fext() == ".idx")
			{
				extract_open_audio_pak(unknown_file.get_ftitle() + ".bag", unknown_file);
			}
			else if (!m_mix_f)
			{
				ShellExecute(m_hWnd, "open", (m_dir + unknown_file.get_fname()).c_str(), NULL, NULL, SW_SHOW);
			} 
			break;
		}
	case ft_shp:
	case ft_shp_ts:
	case ft_vqa:
	case ft_wsa_dune2:
	case ft_wsa:
		{
			Cvideo_decoder* decoder = NULL;
			switch (index.ft)
			{
			case ft_shp:
				{
					Cshp_file f;
					f.load(get_vdata_id(id));
					decoder = f.decoder(get_default_palette());
				}
				break;
			case ft_shp_ts:
				{
					Cshp_ts_file f;
					f.load(get_vdata_id(id));
					decoder = f.decoder(get_default_palette());
				}
				break;
			case ft_vqa:
				{
					Cvqa_file f;
					f.load(get_vdata_id(id));
					decoder = f.decoder();
				}
				break;
			case ft_wsa_dune2:
				{
					Cwsa_dune2_file f;
					f.load(get_vdata_id(id));
					decoder = f.decoder(get_default_palette());
				}
				break;
			case ft_wsa:
				{
					Cwsa_file f;
					f.load(get_vdata_id(id));
					decoder = f.decoder();
				}
				break;
			}
			if (!decoder)
				break;
			Cdlg_shp_viewer dlg;
			dlg.write(decoder);
			dlg.DoModal();
			delete decoder;
			break;
		}
	default:
		{
			if (!m_mix_f)
			{
				Cfname unknown_file(index.name);
				ShellExecute(m_hWnd, "open", (m_dir + unknown_file.get_fname()).c_str(), NULL, NULL, SW_SHOW);
			}
			break;
		}
	}
}

void CXCCMixerView::OnEditSelectAll()
{
	CListCtrl& lc = GetListCtrl();
	for (int index = 0; index < lc.GetItemCount(); index++)
	{
		switch (find_ref(m_index, lc.GetItemData(index)).ft)
		{
		case ft_dir:
		case ft_drive:
		case ft_lnkdir:
			lc.SetItemState(index, 0, LVIS_SELECTED);
			break;
		default:
			lc.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}
