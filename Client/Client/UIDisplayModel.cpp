#include "UIDisplayModel.h"
#include "RenderSystem.h"
#include "UIDialog.h"
#include "Graphics.h"
#include "SkeletonNode.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CUIDisplayModel::CUIDisplayModel()
	:m_pRenderNode(NULL)
	,m_vEye(0.0f, 0.0f, -1.0f)
	,m_vLookAt(0.0f, 0.0f, 0.0f)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CUIDisplayModel::~CUIDisplayModel()
{
	delete m_pRenderNode;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::LoadModel(const char * szFilename)
{
	S_DEL(m_pRenderNode);
	// ----
	m_pRenderNode = new CSkeletonNode;
	m_pRenderNode->load(szFilename);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnFrameMove(double fTime, float fElapsedTime)
{
	// # Update the view matrix (更新视矩阵)
	// # Render the update		(渲染更新)
	// ----
	if(m_pRenderNode != NULL)
	{
		m_pRenderNode->frameMove(Matrix::UNIT, fTime, fElapsedTime);
	}
	// ----
	CUIDisplay::OnFrameMove(fTime, fElapsedTime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnFrameRender(const Matrix & mTransform, double fTime, float fElapsedTime)
{
	CUIDisplay::OnFrameRender(mTransform, fTime, fElapsedTime);
	// ----
	if((IsVisible() == true) && (isStyleVisible() == true))
	{
		Matrix mView;
		Matrix mProj;
		// ----
		Vec3D vUp(0,1,0);
		// ----
		CRenderSystem & R		= GetRenderSystem();
		// ----
		CRect<int> rcViewport	= getViewport();
		CRect<int> rcOldViewport;
		// ----
		R.getViewport(rcOldViewport);
		// ----
		// # Projection matrix (投影矩阵)
		// ----
		float fAspect			= (rcViewport.getWidth() / (float)rcViewport.getHeight());
		// ----
		mProj.MatrixPerspectiveFovLH(PI / 4, fAspect, 0.01f, 128.0f);
		// ----
		R.setProjectionMatrix(mProj);
		// ----
		mView.MatrixLookAtLH(m_vEye, m_vLookAt, vUp);
		// ----
		CShader * pShader = R.GetShaderMgr().getSharedShader();
		// ----
		if (pShader)
		{
			pShader->setFloat("g_fTime", fTime);
			pShader->setMatrix("g_mViewProj", mProj*mView);
			pShader->setMatrix("g_mView", mView);
			pShader->setVec3D("g_vLightDir", m_vEye);
			pShader->setVec3D("g_vEyePot", m_vEye);
		}
		// ----
		// # Set projection matrix & as the matrix (设置投影矩阵&视矩阵)
		// ----
		R.setWorldMatrix(Matrix::UNIT);
		R.setViewMatrix(mView);
		// ----
		// # Back screen to 0 (背屏清0)
		// ----
		R.ClearBuffer(true, false, 0x0);
		// ----
		// # Set window size (设置视窗大小)
		// ----
		R.setViewport(rcViewport);
		// ----
		R.SetSamplerFilter(0, TEXF_LINEAR, TEXF_LINEAR, TEXF_LINEAR);
		R.SetSamplerFilter(1, TEXF_LINEAR, TEXF_LINEAR, TEXF_LINEAR);
		// ----
		if(m_pRenderNode != NULL)
		{
			CTexture* pSceneTexture = NULL;
			// ----
			// # Rendering model (渲染模型)
			// ----
			m_pRenderNode->render(Matrix::UNIT);
			// ----
			pSceneTexture		= m_SceneEffect.getSceneTexture();
			// ----
			pShader->setTexture("g_texScene",pSceneTexture);
			// ----
			R.SetBlendFunc(false);
			// ----
			m_pRenderNode->render(Matrix::UNIT, MATERIAL_BUMP);
		}
		// ----
		// # Reset render states, which will help draw the UI (重置渲染状态，利于UI绘制)
		// ----
		R.SetupRenderState();
		// ----
		// # Reset the window size (重置视窗大小)
		// ----
		R.setViewport(rcOldViewport);
	}

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnMouseMove(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnMouseWheel(POINT point,short wheelDelta)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnLButtonDown(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnLButtonUp(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnRButtonDown(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnRButtonUp(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnMButtonDown(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnMButtonUp(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayModel::OnSize(const CRect<int>& rc)
{
	CUIDisplay::OnSize(rc);
	// ----
	m_SceneEffect.Reset(GetBoundingBox());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------