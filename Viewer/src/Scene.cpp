#include "Scene.h"
#include "MeshModel.h"
#include "Renderer.h"
#include <iostream>
#include <string>

Scene::Scene() :
	active_camera_index_(0),
	active_model_index_(0),
	CamOrWorldView(false)
{
	showAxis = true;
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
	std::cout << std::endl << "A model has been added" << std::endl;
	std::cout << "Model name: " << mesh_model->GetModelName() << std::endl;
	std::cout << "There are now " << mesh_models_.size() << " models" << std::endl;

}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models_[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

std::vector<std::shared_ptr<MeshModel>> Scene::GetModels() const
{
	return mesh_models_;
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

Camera& Scene::GetCamera(int index) const
{
	return *cameras_[index];
}



Camera& Scene::GetActiveCamera() const
{
	return *cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

void Scene::cleanupScene()
{
	mesh_models_.clear();
	cameras_.clear();
}

bool Scene::getShowAxis() const
{
	return showAxis;
}

void Scene::setShowAxis(bool show)
{
	showAxis = show;
}

void Scene::deleteActiveCamera()
{
	cameras_.erase(cameras_.begin() + active_camera_index_);
}

void Scene::deleteActiveModel()
{
	mesh_models_.erase(mesh_models_.begin() + active_model_index_);
}

void Scene::clearCameras()
{
	cameras_.clear();
}

void Scene::clearModels()
{
	mesh_models_.clear();
}


void Scene::SetCamOrWorldView(const bool view) {
	CamOrWorldView = view;
}

bool Scene::GetCamOrWorldView() const
{
	return CamOrWorldView;
}

