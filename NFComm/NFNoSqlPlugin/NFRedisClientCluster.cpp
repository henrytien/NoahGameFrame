/*
            This file is part of:
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang

   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFRedisClient.h"

bool NFRedisClient::CLUSTERNODES(std::vector<std::string>& clusters, bool onlyMasterIP)
{
	NFRedisCommand cmd("CLUSTER");
	cmd << "NODES";

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		if (onlyMasterIP)
		{
			std::vector<std::string> clustersNode;
			StringToVector(std::string(pReply->str, pReply->len), NFREDIS_LF, clustersNode);
			for (auto & node : clustersNode)
			{
				if (node.find("master") != std::string::npos)
				{
					clusters.push_back(node);
				}
			}
		}
		else
		{
			StringToVector(std::string(pReply->str, pReply->len), NFREDIS_LF, clusters);
		}
		return true;
	}

	return false;
}

bool NFRedisClient::CLUSTERINFO(std::string& clusterInfo)
{
	NFRedisCommand cmd("CLUSTER");
	cmd << "INFO";

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		clusterInfo.append(pReply->str, pReply->len);
		return true;
	}

	return false;
}