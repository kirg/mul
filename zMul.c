/*rg@z*/
#include <windows.h>
#include "resource.h"
#pragma comment(linker,"/align:4096")
#pragma comment(linker,"/merge:.rdata=.text")
/*#pragma comment(linker,"/merge:.data=.text")*/
#pragma comment(linker,"/nodefaultlib")



int zMul(const char *m,const char *n,char *p){
	register int i,j;
	int k;
	register unsigned int s;
	int ml,nl,pl;

	pl=(ml=lstrlen(m)-1)+(nl=lstrlen(n)-1);
	if (!p)/*p==NULL;return required bufSize*/
		return pl+2;/*max (possible) length of product string*/
	m+=ml;n+=nl;
	for(s=i=0;i<=pl;p[i++]=s%10+'0',s/=10)
		for(j=(i>ml)?i-ml:0,k=(i<nl)?i:nl;j<=k;++j)
			s+=(m[-i+j]-'0')*(n[-j]-'0');
	if (s)
		p[++pl]=s%10+'0';
	/*for(j=0,i>>=1;j<i;p[j++]^=p[pl-j]^=p[j]^=p[pl-j]);*//*slow?*/
	for(j=0,i>>=1;j<i;++j)/*reverse product string*/{
		s=p[j];
		p[j]=p[pl-j];
		p[pl-j]=s;
	}
	p[++pl]=0;/*null terminate the product string*/
	return pl;/*return the product string's length*/
}


#define totalNumLen	65535
#define maxNumLen	32767

char buf[totalNumLen];
char *m,*n,*p;
char lenStr[6];

int CALLBACK wndproc(HWND,UINT,WPARAM,LPARAM);

#pragma comment(linker,"/entry:main")
int main(){
	return DialogBoxParam(0,(char *)IDD_dlg,0,wndproc,0);
}

int CALLBACK wndproc(HWND hW,UINT msg,WPARAM wp,LPARAM lp){
	switch(msg){
	case WM_COMMAND:
		switch(LOWORD(wp)){
		case IDC_m:
			if (HIWORD(wp)==EN_CHANGE){
				wsprintf(lenStr,"%u",GetWindowTextLength(GetDlgItem(hW,IDC_m)));
				SetWindowText(GetDlgItem(hW,IDC_mLen),lenStr);
				SetWindowText(GetDlgItem(hW,IDC_p),"");
			}
			break;
		case IDC_n:
			if (HIWORD(wp)==EN_CHANGE){
				wsprintf(lenStr,"%u",GetWindowTextLength(GetDlgItem(hW,IDC_n)));
				SetWindowText(GetDlgItem(hW,IDC_nLen),lenStr);
				SetWindowText(GetDlgItem(hW,IDC_p),"");
			}
			break;
		case IDC_p:
			if (HIWORD(wp)==EN_CHANGE){
				wsprintf(lenStr,"%u",GetWindowTextLength(GetDlgItem(hW,IDC_p)));
				SetWindowText(GetDlgItem(hW,IDC_pLen),lenStr);
			}
			break;
		case IDOK:
			n=m+1+GetWindowText(GetDlgItem(hW,IDC_m),m=buf,maxNumLen);
			p=n+1+GetWindowText(GetDlgItem(hW,IDC_n),n,maxNumLen-(n-m));
			if (m[0] || n[0]){
				SetWindowText(hW,"zMul processing ...");
				if (!n[0]){
					SetWindowText(GetDlgItem(hW,IDC_n),m);
					UpdateWindow(hW);
					msg=GetTickCount();
					zMul(m,m,p);
					msg=GetTickCount()-msg;
				}
				else
					if (!m[0]){
						SetWindowText(GetDlgItem(hW,IDC_m),n);
						UpdateWindow(hW);
						msg=GetTickCount();
						zMul(n,n,p);
						msg=GetTickCount()-msg;
					}
					else{
						msg=GetTickCount();
						zMul(m,n,p);
						msg=GetTickCount()-msg;
					}
				SetWindowText(GetDlgItem(hW,IDC_p),p);
				wsprintf(buf,"zMul (%u milliseconds)",msg);
				SetWindowText(hW,buf);
				SetTimer(hW,1,5000,0);
				UpdateWindow(hW);
			}
			else
				MessageBeep(~0);
			break;
		case IDCANCEL:
			EndDialog(hW,0);
			/*break;*/
		}
		break;
	case WM_TIMER:
		KillTimer(hW,1);
		SetWindowText(hW,"zMul by _zyxWare_");
		break;
/*	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hW,IDC_m),EM_SETLIMITTEXT,2*maxNumLen,0);
		SendMessage(GetDlgItem(hW,IDC_n),EM_SETLIMITTEXT,2*maxNumLen,0);
		SendMessage(GetDlgItem(hW,IDC_p),EM_SETLIMITTEXT,2*maxNumLen,0);
		/*break;*/
	}
	return 0;
}