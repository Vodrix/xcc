#pragma once

#include <cc_file.h>
#include <mix_file.h>
#include <mix_file_rd.h>
#include <palette.h>
#include "palette_filter.h"

struct t_text_cache_entry
{
	CRect text_extent;
	string t;
};

using t_text_cache = vector<t_text_cache_entry>;

class CXCCFileView : public CScrollView
{
protected:
	CXCCFileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CXCCFileView)
public:
	bool can_auto_select();
	void auto_select();
	void close_f();
	const t_palette_entry* get_default_palette();
	void load_color_table(const t_palette palette, bool convert_palette);
	void draw_image8(const byte* s, int cx_s, int cy_s, CDC* pDC, int x_d);
	void draw_image24(const byte* s, int cx_s, int cy_s, CDC* pDC);
	void draw_image32(const byte* s, int cx_s, int cy_s, CDC* pDC);
	void draw_image48(const byte* s, int cx_s, int cy_s, CDC* pDC);
	void draw_image64(const byte* s, int cx_s, int cy_s, CDC* pDC);
	void draw_info(string n, string d);
	// void set_game(t_game);
	void open_f(int id, Cmix_file& mix_f, t_game game, t_palette palette);
	void open_f(const string& name);
	void post_open(Ccc_file& f);

	//{{AFX_VIRTUAL(CXCCFileView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

protected:
	virtual ~CXCCFileView();
	//{{AFX_MSG(CXCCFileView)
	afx_msg void OnDisable(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	COLORREF  m_colour = RGB(40, 40, 40);
	CRect			clientRect;
	bool			m_can_pick;
	CRect			m_clip_rect;
	DWORD			m_color_table[256];
	int				m_cx;
	int				m_cy;
	int				m_cx_dib;
	Cvirtual_binary	m_data;
	CDC*			m_dc;
	string			m_fname;
	t_game			m_game;
	HBITMAP			mh_dib;
	DWORD*			mp_dib;
	CFont			m_font;
	t_file_type		m_ft;
	int				m_id;
	bool			m_is_open = false;
	t_palette_entry*	m_palette;
	Cpalette_filter m_palette_filter;
	long long		m_size;
	t_text_cache	m_text_cache;
	bool			m_text_cache_valid;
	int				m_x;
	int				m_y;
	int				m_y_inc;
	CBrush test_brush;
	static const int offset = 4;
};
