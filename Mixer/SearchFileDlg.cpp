#include "stdafx.h"
#include "MainFrm.h"
#include "SearchFileDlg.h"
#include "string_conversion.h"

CSearchFileDlg::CSearchFileDlg(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CSearchFileDlg::IDD, pParent, "find_file_dlg")
{
	m_reg_key = "find_file_dlg";
	//{{AFX_DATA_INIT(CSearchFileDlg)
	//}}AFX_DATA_INIT
	m_filename = AfxGetApp()->GetProfileString(m_reg_key, "file_name");
}

void CSearchFileDlg::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchFileDlg)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchFileDlg, ETSLayoutDialog)
	//{{AFX_MSG_MAP(CSearchFileDlg)
	ON_BN_CLICKED(IDOK, OnFind)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSearchFileDlg::set(CMainFrame* main_frame)
{
	m_main_frame = main_frame;
}

BOOL CSearchFileDlg::OnInitDialog() 
{
	CreateRoot(VERTICAL)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_FILENAME_STATIC, NORESIZE)
			<< item(IDC_FILENAME, GREEDY)
			)
		<< item(IDC_LIST, GREEDY)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemGrowing(HORIZONTAL)
			<< item(IDOK, NORESIZE)
			<< item(IDCANCEL, NORESIZE)
			);
	ETSLayoutDialog::OnInitDialog();
	m_list.InsertColumn(0, "Name");
	m_list.set_size(0);
	return true;
}

void CSearchFileDlg::find(Cmix_file& f, string file_name, string mix_name, int mix_id, int sub_mix_id)
{
	for (int i = 0; i < f.get_c_files(); i++)
	{
		const int id = f.get_id(i);
		string name = f.get_name(id);
		if (name.empty())
		{
			name = nh(8, id);
			if (Cmix_file::get_id(f.get_game(), file_name) == id)
				add(mix_name + " - " + name, mix_id, id, sub_mix_id);
		}
		else if (fname_filter(name, file_name))
			add(mix_name + " - " + name, mix_id, id, sub_mix_id);
		if (f.get_type(id) == ft_mix)
		{
			Cmix_file fg;
			if (!fg.open(id, f))
			{
				find(fg, file_name, mix_name + " - " + name, mix_id, i);
			}
		}
	}
	for (auto& i : m_main_frame->mix_map_list())
	{
		if (i.second.parent != mix_id)
			continue;
		Cmix_file g;
		if (!g.open(i.second.id, f))
			find(g, file_name, mix_name + " - " + (i.second.name.empty() ? nh(8, i.second.id) : i.second.name), i.first);
	}
}

void CSearchFileDlg::find(const map<int, t_index_entry>& t_map, const string& post, const string& dir)
{
	for (auto& i : t_map)
	{
		if (i.second.name.empty())
			continue;
		string fname = dir.rfind('\\') == string::npos ? (dir + '\\' + i.second.name) : (dir + i.second.name);
		Cmix_file f;
		if (!f.open(fname))
		{
			find(f, get_filename(), i.second.name + post, i.first);
		}
		else if (i.second.ft == ft_mix)
		{
			Cmix_file_rd f_rd;
			if (!f_rd.open(fname))
			{
				find(f_rd, get_filename(), i.second.name + post, i.first);
			}
		}
	}
}

void CSearchFileDlg::OnFind() 
{
	if (UpdateData(true))
	{
		CWaitCursor wait;
		m_list.DeleteAllItems();
		m_map.clear();
		find(m_main_frame->left_mix_pane()->t_index_list(), " (1)", m_main_frame->left_mix_pane()->current_dir());
		m_sepindex = m_map.size();
		find(m_main_frame->right_mix_pane()->t_index_list(), " (2)", m_main_frame->right_mix_pane()->current_dir());
		m_list.SetItemCount(m_map.size());
		for (auto& i : m_map)
			m_list.InsertItemData(i.first);
	}
}

void CSearchFileDlg::add(string name, int mix_id, int file_id, int sub_mix_id)
{
	t_map_entry& e = m_map[m_map.size()];
	e.name = name;
	e.id = file_id;
	e.parent = mix_id;
	e.parent_parent = sub_mix_id;
}

void CSearchFileDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int index = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (index != -1)
		open_mix(m_list.GetItemData(index));
	*pResult = 0;
}

void CSearchFileDlg::open_mix(int id)
{
	const t_map_entry& e = m_map[id];
	//m_main_frame->left_mix_pane()->open_location_mix(m_main_frame->mix_map_list().find(e.parent), e.id);
	if (id < m_sepindex) // left
	{
		m_main_frame->left_mix_pane()->open_location_mix(e.parent, e.parent_parent, e.id);
	}
	else
	{
		m_main_frame->right_mix_pane()->open_location_mix(e.parent, e.parent_parent, e.id);
	}
	EndDialog(IDCANCEL);
}

void CSearchFileDlg::OnDestroy() 
{
	ETSLayoutDialog::OnDestroy();
	AfxGetApp()->WriteProfileString(m_reg_key, "file_name", m_filename);
}

void CSearchFileDlg::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	int id = pDispInfo->item.lParam;
	const t_map_entry& e = find_ref(m_map, id);
	string& buffer = m_list.get_buffer();
	buffer = e.name;
	pDispInfo->item.pszText = const_cast<char*>(buffer.c_str());
	*pResult = 0;
}
