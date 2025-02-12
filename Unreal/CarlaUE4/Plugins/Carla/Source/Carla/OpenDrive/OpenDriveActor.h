// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once//预处理指令，用于确保这个头文件在单个编译单元中只被包含（include）一次。这有助于防止多重包含问题

#include "GameFramework/Actor.h"

#include "Traffic/RoutePlanner.h"//用于规划交通路线的类或函数的定义
#include "Vehicle/VehicleSpawnPoint.h"//用于定义车辆生成点的类或函数

#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "CoreMinimal.h"//包含了Unreal Engine核心功能的一些基本定义和宏

//由Unreal Engine的Unreal Header Tool（UHT）自动生成的，包含了OpenDriveActor类的反射系统所需的代码。反射系统允许Unreal Engine在运行时动态地查询和操作类的属性和方法
#include "OpenDriveActor.generated.h"

UCLASS()//这是一个宏，用于在Unreal Engine中声明一个类
class CARLA_API AOpenDriveActor : public AActor // 定义AOpenDriveActor类，继承自AActor
{
  GENERATED_BODY()

protected:

  ///Billboard组件用于显示图标精灵。
  UBillboardComponent *SpriteComponent;

  /// Billboard组件使用的精灵
  UTexture2D *SpriteTexture;

private:

  UPROPERTY() ///定义一个数组属性，存储VehicleSpawnPoint对象
  TArray<AVehicleSpawnPoint *> VehicleSpawners;

#if WITH_EDITORONLY_DATA
  /// 使用OpenDrive文件生成道路网络（文件名与当前.umap相同）
  ///
  UPROPERTY(Category = "Generate", EditAnywhere)
  bool bGenerateRoutes = false;
#endif // WITH_EDITORONLY_DATA

  /// 车辆行驶的路径点之间的距离。
  UPROPERTY(Category = "Generate", EditAnywhere, meta = (ClampMin = "0.01", UIMin = "0.01"))
  float RoadAccuracy = 2.f;

  ///触发器的高程
  UPROPERTY(Category = "Generate", EditAnywhere, meta = (ClampMin = "0.01", UIMin = "0.01"))
  float TriggersHeight = 100.f;

#if WITH_EDITORONLY_DATA ///如果编译时包含编辑器数据。
  /// 移除之前生成的道路网络，如果需要也会移除生成器
  /// 
  UPROPERTY(Category = "Generate", EditAnywhere)
  bool bRemoveRoutes = false;
#endif // WITH_EDITORONLY_DATA

  /// 如果为true，在生成路径时放置生成器。
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bAddSpawners = false;

  /// 如果为true，在交叉路口也放置生成器
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bOnIntersections = false;

  /// 确定生成器放置的高度，相对于每个RoutePlanner。
  /// 
  UPROPERTY(Category = "Spawners", EditAnywhere)
  float SpawnersHeight = 300.f;

#if WITH_EDITORONLY_DATA
  ///如果需要，移除已经放置的生成器。
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bRemoveCurrentSpawners = false;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITORONLY_DATA
  /// 显示/隐藏额外的调试信息
  UPROPERTY(Category = "Debug", EditAnywhere)
  bool bShowDebug = true;
#endif // WITH_EDITORONLY_DATA

public:

  AOpenDriveActor(const FObjectInitializer &ObjectInitializer);/// 构造函数

  void BuildRoutes();/// 构建路径

  void BuildRoutes(FString MapName); ///根据地图名称构建路径

  /// 移除这个类之前生成的所有ARoutePlanner和VehicleSpawners以避免重叠
  ///
  void RemoveRoutes();

  void DebugRoutes() const;// 调试路径

  void RemoveDebugRoutes() const;

#if WITH_EDITOR
  void PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent);// 属性改变后的处理

#endif // WITH_EDITOR

  void AddSpawners(); // 添加生成器

  void RemoveSpawners(); // 移除生成器

  UPROPERTY() // 定义一个数组属性，存储RoutePlanner对象
  TArray<ARoutePlanner *> RoutePlanners;
};
