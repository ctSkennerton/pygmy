//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================


#include "../core/MetadataInfo.hpp"
#include "../core/NodePhylo.hpp"

#include "../utils/Tree.hpp"

using namespace utils;
using namespace pygmy;

const float FieldInfo::NA = -FLT_MAX;

std::vector<QString> MetadataInfo::GetFields()
{
    std::vector<QString> fields;

    typedef std::pair<QString, FieldInfo> pair_t;
	foreach(pair_t data, m_metadataInfo)
	{	
		fields.push_back(data.first);
	}

	return fields;
}

void MetadataInfo::AddMetadata(const std::map<QString, QString>& metadata)
{
    typedef std::pair<QString, QString> pair_t;
    for(pair_t data : metadata)
	{
        std::map<QString, FieldInfo>::iterator it = m_metadataInfo.find(data.first);
		if(it == m_metadataInfo.end())
		{
            std::set<QString> values;
			if(!IsMissingData(data.second)) 
			{
				values.insert(data.second);

				// add new field
                bool ok;
                float value = data.second.toFloat(&ok);
                if(ok)
				{
					m_metadataInfo[data.first] = FieldInfo(value, value, FieldInfo::NUMERICAL, values); 
				}
				else
				{
					m_metadataInfo[data.first] = FieldInfo(FieldInfo::NA, FieldInfo::NA, FieldInfo::CATEGORICAL, values); 
				}
			}
		}
		else
		{
			// update field info
			FieldInfo& fieldInfo = it->second;
			if(!IsMissingData(data.second)) 
			{
				fieldInfo.values.insert(data.second);

                bool ok;
                float value = data.second.toFloat(&ok);
                if(ok)
				{

					if(value < fieldInfo.minValue)
							fieldInfo.minValue = value;
					if(value > fieldInfo.maxValue)
						fieldInfo.maxValue = value;

					fieldInfo.dataType = FieldInfo::NUMERICAL;
				}
				else
				{
					fieldInfo.dataType = FieldInfo::CATEGORICAL;				
				}
			}
		}
	}
}

void MetadataInfo::SetMetadata(utils::Tree<NodePhylo>::Ptr tree)
{
	Clear();

	std::vector< NodePhylo* > leaves = tree->GetLeaves();
    for(NodePhylo* leaf : leaves)
	{
		AddMetadata(leaf->GetMetadata());
	}
}

bool MetadataInfo::IsMissingData(const QString& value)
{
    return value.isEmpty() || value == "N/A" || value == "-" || value == "NULL";
}
