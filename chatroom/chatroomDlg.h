
// chatroomDlg.h: 头文件
//

#pragma once

// CchatroomDlg 对话框
class CchatroomDlg : public CDialogEx
{
// 构造
public:
	CchatroomDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_input_edit;
	CEdit m_showchatcontent_edit;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
