#pragma once


// CImgScalingDlg 대화 상자

class CImgScalingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImgScalingDlg)
private:
	CStatic m_EditScaleRate;

	double dScaleRate;
	int nInterpolationType;

public:
	CImgScalingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImgScalingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCALEDIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_Interp;
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioClicked(UINT msg);

	double GetScaleRate();
	int GetInterpType();
};
