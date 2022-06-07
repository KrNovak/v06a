#include "main.h"
#include "rc.h"

int size_dialog::idd() const {
	return IDD_SIZE; 
}

bool size_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, x);
	set_int(IDC_EDIT2, y);
	return true;
}

bool size_dialog::on_ok(){
	try {
		x = get_int(IDC_EDIT1);
		y = get_int(IDC_EDIT2);
	}
	catch(...) {
		return false;
	}
	return true;
}


void main_window::on_paint(HDC hdc){
	HBRUSH hbr = CreateSolidBrush(color);
	RECT rect;
	GetClientRect(*this, &rect);
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetViewportExtEx(hdc, rect.right, rect.bottom, 0);
	SetWindowExtEx(hdc, x, y, 0);
	for (int i = 0; i < x; i++) {
		for (int j = (i & 1); j < y; j += 2) {
			RECT rc = { i, j, i + 1, j + 1 };
			FillRect(hdc, &rc, hbr);
		}
	}
	DeleteObject(hbr);
}



void main_window::on_command(int id){
	switch(id){
		case ID_SIZE:
		{
			size_dialog sDlg;
			sDlg.x = x;
			sDlg.y = y;
			if (sDlg.do_modal(0, *this) == IDOK) {
				x = sDlg.x;
				y = sDlg.y;
				InvalidateRect(*this, NULL, true);
			}
			break;
		}
		case ID_COLOR:
		{
			static COLORREF c[100] = { 0 };
			CHOOSECOLOR ch_color;
			ZeroMemory(&ch_color, sizeof ch_color);
			ch_color.rgbResult = color;
			ch_color.lpCustColors = c;
			ch_color.hwndOwner = *this;
			ch_color.lStructSize = sizeof ch_color;
			ch_color.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&ch_color)) {
				color = ch_color.rgbResult;
				InvalidateRect(*this, NULL, true);
			}
			break;
		}
		case ID_EXIT: {
			DestroyWindow(*this);
			break;
		}
			
	}
}


void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.run();
}
