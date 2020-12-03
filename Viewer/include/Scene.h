#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"

class Scene {
public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	std::vector<std::shared_ptr<MeshModel>> GetModels()const;
	
	void AddCamera(const std::shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index) const;
	Camera& GetActiveCamera() const;


	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	void SetCamOrWorldView(const bool view);
	bool GetCamOrWorldView() const;
	

	void cleanupScene();

	bool getShowAxis() const ;

	void setShowAxis(bool show);
	bool showAxis;

	void deleteActiveCamera();
	void deleteActiveModel();
	void clearCameras();
	void clearModels();
private:
	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<std::shared_ptr<Camera>> cameras_;
	
	int active_camera_index_;
	int active_model_index_;
	bool CamOrWorldView; //true for camera view, false for World
};