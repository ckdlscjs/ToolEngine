#pragma once
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")
namespace fbxsample
{
    /* Tab character ("\t") counter */

    class FBXSample
    {
    public:
        void sample()
        {
            const char* szFileName = "../../data/fbx/greystone.fbx";
            FbxManager* pFbxManager = FbxManager::Create();
            FbxImporter* pImporter = FbxImporter::Create(pFbxManager, "");
            pImporter->Initialize(szFileName, -1, nullptr);

            FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "Scene");

            pImporter->Import(pFbxScene);

            FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
            
            int childCount = pFbxRootNode->GetChildCount();

            PrintNode(pFbxRootNode, 0);
       

            pFbxRootNode->Destroy();
            pFbxScene->Destroy();
            pImporter->Destroy();
            pFbxManager->Destroy();
        }


        /**
         * Return a string-based representation based on the attribute type.
         */
        FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
            switch (type) {
            case FbxNodeAttribute::eUnknown: return "unidentified";
            case FbxNodeAttribute::eNull: return "null";
            case FbxNodeAttribute::eMarker: return "marker";
            case FbxNodeAttribute::eSkeleton: return "skeleton";
            case FbxNodeAttribute::eMesh: return "mesh";
            case FbxNodeAttribute::eNurbs: return "nurbs";
            case FbxNodeAttribute::ePatch: return "patch";
            case FbxNodeAttribute::eCamera: return "camera";
            case FbxNodeAttribute::eCameraStereo: return "stereo";
            case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
            case FbxNodeAttribute::eLight: return "light";
            case FbxNodeAttribute::eOpticalReference: return "optical reference";
            case FbxNodeAttribute::eOpticalMarker: return "marker";
            case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
            case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
            case FbxNodeAttribute::eBoundary: return "boundary";
            case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
            case FbxNodeAttribute::eShape: return "shape";
            case FbxNodeAttribute::eLODGroup: return "lodgroup";
            case FbxNodeAttribute::eSubDiv: return "subdiv";
            default: return "unknown";
            }
        }

        /**
         * Print an attribute.
         */

        void printTab(int depth)
        {
            for (int i = 0; i < depth; i++)
                printf("#");
        }
        void PrintAttribute(FbxNodeAttribute* pAttribute) {
            if (!pAttribute) return;
            FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
            FbxString attrName = pAttribute->GetName();
            //PrintTabs();
            // Note: to retrieve the character array of a FbxString, use its Buffer() method.
            printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
        }

        /**
         * Print a node, its attributes, and all its children recursively.
         */
        void PrintNode(FbxNode* pNode, int depth) {
           
            const char* nodeName = pNode->GetName();
            FbxMesh* pMesh = pNode->GetMesh();
            FbxDouble3 translation = pNode->LclTranslation.Get();
            FbxDouble3 rotation = pNode->LclRotation.Get();
            FbxDouble3 scaling = pNode->LclScaling.Get();

            // Print the contents of the node.
            printTab(depth);
            printf("node name='%s'\n", nodeName);

            printTab(depth);
            // Print the node's attributes.
            for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
                PrintAttribute(pNode->GetNodeAttributeByIndex(i));

            int childCount = pNode->GetChildCount();
            printTab(depth);
            printf("-------------childCount------------- : %d\n\n", childCount);
            // Recursively print the children.
            for (int j = 0; j < childCount; j++)
            {
                printTab(depth+1);
                printf("childIdx : %d\n", j);
                PrintNode(pNode->GetChild(j), depth+1);
            }
       
        }
    };

}
