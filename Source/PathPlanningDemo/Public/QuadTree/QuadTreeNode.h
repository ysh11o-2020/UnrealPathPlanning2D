// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

/**
 * 
 */

class QuadTreeNode : public TSharedFromThis<QuadTreeNode>
{
public:
	FVector Center;	//中心点
	FVector Extend;	//扩展尺寸
	bool bIsLeaf;	//是否为叶子节点
	int32 Depth = 0;
	int32 MaxCount = 4;

	TArray<AActor*> Objects;
	static UWorld* WorldObject;
	bool bInRange;

	TSharedPtr<QuadTreeNode> Root;
	TArray<TSharedPtr<QuadTreeNode>> ChildrenNode;

	TWeakPtr<QuadTreeNode> GlobalRoot;
	
public:
	QuadTreeNode():
	Center(FVector::ZeroVector),
	Extend(FVector(10000,10000,0)),
	bIsLeaf(false),
	Depth(0),
	MaxCount(4)
	{
		Objects.Empty();
		WorldObject = nullptr;
		bInRange = false;

		Root = nullptr;
		ChildrenNode.Empty();
		GlobalRoot = nullptr;
	}
	

	QuadTreeNode(const FVector& InCenter, const FVector& InExtend, int32 InDepth, const TSharedPtr<QuadTreeNode>& InRoot = nullptr): Center(InCenter),Extend(InExtend),Depth(InDepth)
	{
		Root = InRoot;
		bIsLeaf = true;
		ChildrenNode.Init(nullptr,4);
		bInRange = false;

		// if (!Root.IsValid())
		// {
		// 	GlobalRoot = this->AsShared();
		// }
		// else
		// {
		// 	GlobalRoot = Root -> GlobalRoot;
		// }
	}
	
	~QuadTreeNode(){
		Root = nullptr;
		GlobalRoot = nullptr;
		Objects.Empty();
		ChildrenNode.Empty();
	}


	bool IsNotUsed() const
	{
		return bIsLeaf && Objects.Num() <= 0;
	}

	//方形区域与圆形求交(AABB算法)
	bool InterSection(const FVector& CircleCenter, float Radius) const
	{
		float MaxX = Center.X + Extend.X;
		float MinX = Center.X - Extend.X;
		float MaxY = Center.Y + Extend.Y;
		float MinY = Center.Y - Extend.Y;

		float ClosestX = FMath::Clamp(CircleCenter.X,MinX,MaxX);
		float ClosestY = FMath::Clamp(CircleCenter.Y,MinY,MaxY);

		return (ClosestX - CircleCenter.X) * (ClosestX - CircleCenter.X) + (ClosestY - CircleCenter.Y) * (ClosestY - CircleCenter.Y) <= Radius * Radius;
	}
	
	//点是否在区域内
	bool InterSection(const FVector& Point) const
	{
		FBox2D NodeBox(
			FVector2D(Center.X - Extend.X,Center.Y - Extend.Y),
			FVector2D(Center.X + Extend.X,Center.Y + Extend.Y));
		return NodeBox.IsInside(FVector2D(Point));
	}

	//点是否在指定区域内
	bool InterSection(const FVector& PointMin, const FVector& PointMax, const FVector& Point) const
	{
		FBox2D NodeBox(FVector2D(PointMin.X, PointMin.Y),
			FVector2D(PointMax.X, PointMax.Y));
		return NodeBox.IsInside(FVector2D(Point));
	}
	
	//插入对象
	void InsertObject(AActor* InObject)
	{
	Objects.Add(InObject);

	if (bIsLeaf && Objects.Num() <= MaxCount) return;
		
	float DirX[4] = {1,-1,-1,1};
	float DirY[4] = {1,1,-1,-1};

	//超过上限个数，则创建子节点，或者不再是叶子节点
	bIsLeaf = true;
	for (auto Obj : Objects)
	{
		for (int32 i = 0; i < 4; i++)
		{
			//子节点的中心和扩展
			FVector ChildCenter = Center + 0.5f * FVector(Extend.X * DirX[i],Extend.Y * DirY[i],0);
			FVector ChildExtend = 0.5f * Extend;

			//子区域的边缘点
			FVector Point = Center + FVector(Extend.X*DirX[i],Extend.Y*DirY[i],0);
			FVector PointMin = Point.ComponentMin(Center);
			FVector PointMax = Point.ComponentMax(Center);

			if (InterSection(PointMin,PointMax,InObject->GetActorLocation()))
			{
				if (!ChildrenNode[i].IsValid())
				{
					if (!Root.IsValid())
					{
						Root = this->AsShared();
						ChildrenNode[i] = MakeShared<QuadTreeNode>(ChildCenter,ChildExtend,Depth+1,Root);
					}
					if (ChildrenNode[i].IsValid() && Obj!=nullptr)
					{
						ChildrenNode[i]->InsertObject(Obj);
					}
				}
			}
		}
	}
	Objects.Empty();
	}

	//绘制区域边界
	void DrawBound(float Time = 0.02f, float Thickness = 2.0f)
	{
		if (WorldObject)
		{
			FLinearColor DrawColor = bInRange ? FLinearColor::Green : FLinearColor::Red;
			FVector DrawCenter = Center + (bInRange?FVector(0,0,8):FVector(0,0,5));
			UKismetSystemLibrary::DrawDebugBox(WorldObject,DrawCenter,Extend+FVector(0,0,1),DrawColor,FRotator::ZeroRotator,Time,Thickness);
		}
	}

	//判断电池是否在扫描器的范围内
	void TraceObjectInRange(AActor* TraceActor,float Radius)
	{
		if (TraceActor==nullptr || Radius<=0) return;
		FVector ObjectCenter = TraceActor->GetActorLocation();
		if (InterSection(ObjectCenter,Radius))
		{
			bInRange = true;
			if (bIsLeaf)
			{
				for (AActor* Obj : Objects)
				{
					ObjectCenter.Z = Obj->GetActorLocation().Z;
					bool bCanActive = FVector::Dist(ObjectCenter,Obj->GetActorLocation()) <= Radius;

					//物品激活操作
				}
			}
			else
			{
				for (auto& Node:ChildrenNode)
				{
					if (Node.IsValid())
					{
						Node->TraceObjectInRange(TraceActor,Radius);
					}
				}
			}
		}
		else
		{
			TraceObjectOutRange(ObjectCenter,Radius);
		}
	}

	//对不在扫描器范围内的电池进行操作
	void TraceObjectOutRange(FVector ObjectCenter, float Radius)
	{
		bInRange = false;
		for (AActor* Obj : Objects)
		{
			//物品失效逻辑
		}
		for (auto& Node : ChildrenNode)
		{
			if (Node.IsValid())
			{
				Node->TraceObjectOutRange(ObjectCenter,Radius);
			}
		}
	}

	//更新状态
	void UpdateState()
	{
		DrawBound(1/UKismetSystemLibrary::GetFrameCount());	//根据帧数绘制

		if (!bIsLeaf)
		{
			for (auto& Node : ChildrenNode)
			{
				if (Node.IsValid())
				{
					Node->UpdateState();
					if (Node->IsNotUsed())
					{
						Node.Reset();
					}
				}
			}

			int32 Count = 4;
			for (auto& Node:ChildrenNode)
			{
				if (!Node.IsValid())
				{
					Count --;
				}
			}
			if (Count == 0)
			{
				bIsLeaf = true;
			}
			else
			{
				return;
			}

			//如果为叶子结点，更新物体是否在区域内，不在区域内则移除，并重新插入
			if (bIsLeaf && Objects.Num() > 0)
			{
				int32 i = 0;
				while (i < Objects.Num())
				{
					if (!InterSection(Objects[i]->GetActorLocation()))
					{
						AActor* Battery = Objects[i];
						Objects.Swap(i,Objects.Num()-1);
						Objects.Pop();
						Root->InsertObject(Battery);
						continue;
					}
					i++;
				}
			}
		}
	}
};

// 定义静态成员变量（初始化为 nullptr）
UWorld* QuadTreeNode::WorldObject = nullptr;