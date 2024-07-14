#include "http/ws_recv_handler/WSRequestManager.hpp"

#include <string>

#include "dto/DtoWSErrorResponse.hpp"
#include "http/ws_recv_handler/WSFileListTask.hpp"

namespace tr {
    WSRequestManager::WSRequestManager() {
		this->initFilter();
		this->initTask();
	}

    void WSRequestManager::run(WSClientPtr client, std::shared_ptr<Json::Value> json) {
        if (client->isConnected() == false) {
            return;
        }

        // filter
        for (auto filter : this->filters) {
            if (filter->filter(client, json) == false) {
                return;
            }
        }

        std::string taskType;
        try {
            taskType = (*json)["task"].asString();
        } catch (Json::Exception& e) {
            client->sendResponse(DtoWSErrorResponse::createErrorMessage("type is not string"));
			return;
		}

        // task
        if (this->tasks.find(taskType) != this->tasks.end()) {
            this->tasks[taskType]->task(client, json);
        } else {
            client->sendResponse(DtoWSErrorResponse::createErrorMessage("Invalid task type"));
        }
    }

    void WSRequestManager::initFilter() {}

    void WSRequestManager::initTask() {
        this->addTask(WSFileListTask::create());
    }

    void WSRequestManager::addFilter(WSRequestFilterPtr filter) {
        this->filters.push_back(filter);
    }

    void WSRequestManager::addTask(WSRequestTaskPtr task) {
        this->tasks[task->getTaskType()] = task;
    }
}
