//
// Created by jun on 2021/06/14.
//

#ifndef ANDCG3DVIEWER_IMPORT_FBX_H
#define ANDCG3DVIEWER_IMPORT_FBX_H

namespace fbx {

class import_fbx {
public:
	static bool load(const std::vector<char>& MqoModelData);
};

}	/* namespace fbx */

#endif //ANDCG3DVIEWER_IMPORT_FBX_H
