#include <iostream>
#include <sstream>
#include "RSJparser.tcc"
#include "Map.hpp"

Layer *createLayer(std::string layerString, int width,int heigth){
    Layer *layer = new Layer();

    int **matrix = new int*[width];
    for(int i = 0; i < width;++i){
        matrix[i] = new int[heigth];
        //TODO could be remove, ask a pro
        // for(int j = 0; j < heigth;++j){
        //     matrix[i][j] = 0;
        // }
    }

    layer->matrix = matrix;

    std::istringstream iss(layerString);
	int currWidth=0,currHeigth=0;
	for(std::string line; std::getline (iss,line,',');){
		layer->matrix[currWidth][currHeigth] = stoi(line);
        ++currWidth;
        if(currWidth >= width){
            ++currHeigth;
            currWidth = 0;
        }
	}

    return layer;
}

Layer *createCollisionLayer(RSJresource layerResource){
    std::cerr<<"Parsing collision layer"<<std::endl;
    std::string layerString = layerResource["data"].as<std::string>();layerString = layerString.substr(1,layerString.length()-2);
    
    int width = layerResource["width"].as<int>();
    int heigth = layerResource["height"].as<int>();

    Layer *layer = createLayer(layerString,width,heigth);
    layer->name = layerResource["name"].as<std::string>();

    return layer;
}

Layer *createTopCollisionLayer(RSJresource layerResource){
    std::cerr<<"Parsing TOP collision layer"<<std::endl;
    std::string layerString = layerResource["data"].as<std::string>();layerString = layerString.substr(1,layerString.length()-2);
    
    int width = layerResource["width"].as<int>();
    int heigth = layerResource["height"].as<int>();

    Layer *layer = createLayer(layerString,width,heigth);
    layer->name = layerResource["name"].as<std::string>();

    return layer;
}

Layer *createEnemiesBoundLayer(RSJresource layerResource){
    std::cerr<<"Parsing enemies collision layer"<<std::endl;
    std::string layerString = layerResource["data"].as<std::string>();layerString = layerString.substr(1,layerString.length()-2);
    
    int width = layerResource["width"].as<int>();
    int heigth = layerResource["height"].as<int>();

    Layer *layer = createLayer(layerString,width,heigth);
    layer->name = layerResource["name"].as<std::string>();

    return layer;
}

void createObjects(RSJresource objectsResource, Map *map){
    for(auto it = objectsResource["objects"].as_array().begin();it!=objectsResource["objects"].as_array().end(); ++it){
        RSJresource thisObjectResource = it->as<RSJresource>();
        if(thisObjectResource["name"].as<std::string>() == "exit"){
            double exitX = thisObjectResource["x"].as<double>();
            double exitY = thisObjectResource["y"].as<double>();
            double exitWidth = thisObjectResource["width"].as<double>();
            double exitHeight = thisObjectResource["height"].as<double>();

            map -> exit = sf::FloatRect(exitX,exitY,exitWidth,exitHeight);
        }else if(thisObjectResource["name"].as<std::string>() == "start"){
            double startX = thisObjectResource["x"].as<double>();
            double startY = thisObjectResource["y"].as<double>();
            double startWidth = thisObjectResource["width"].as<double>();
            double startHeight = thisObjectResource["height"].as<double>();

            map -> start = sf::FloatRect(startX,startY,startWidth,startHeight);
        }else if(thisObjectResource["name"].as<std::string>() == "enemy"){
            double x = thisObjectResource["x"].as<double>();
            double y = thisObjectResource["y"].as<double>();
            map->enemiesPos.push_back(Point(x,y));
        }else if(thisObjectResource["name"].as<std::string>() == "ring"){
            double x = thisObjectResource["x"].as<double>();
            double y = thisObjectResource["y"].as<double>();
            map->ringPos.push_back(Point(x,y));
        }else{
            std::cerr<<"Can't parse object : "<<thisObjectResource["name"].as<std::string>()<<std::endl;
        }
    }
}

Map *createMapFromJSON(std::string path){
    std::ifstream my_fstream (path);
    if(!my_fstream)throw std::runtime_error("No file : " + path);
    std::string levelString = "";
    for(std::string line; std::getline (my_fstream,line);){
        levelString += line;
    }
    RSJresource json_file_resource (levelString);

    int width, heigth;
    width = json_file_resource["width"].as<int>();
    heigth = json_file_resource["height"].as<int>();

    Map *newMap = new Map(width,heigth);

    /*New code starts here*/
    for(auto it = json_file_resource["layers"].as_array().begin();it!=json_file_resource["layers"].as_array().end(); ++it){
        RSJresource layerResource = it->as<RSJresource>();
        if(layerResource["name"].as<std::string>() == "CollisionTiles"){
            newMap->layers.push_back(createCollisionLayer(layerResource));
        }else if(layerResource["name"].as<std::string>() == "Objects"){
            createObjects(layerResource,newMap);
        }else if(layerResource["name"].as<std::string>() == "EnemyCollision"){
            newMap->layers.push_back(createEnemiesBoundLayer(layerResource));
        }else if(layerResource["name"].as<std::string>() == "TopCollisionTiles"){
            newMap->layers.push_back(createTopCollisionLayer(layerResource));
        }else{
            std::cout<< layerResource["name"].as<std::string>()<<" no function to handle that layer"<<std::endl;
        }
    }


    return newMap;
}
