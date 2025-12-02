#include "Head.h"
namespace CS_SDK//开发者工具库(防止和基础函数冲突)
{
#define MAX_PLAYER 128//全局支持的最大玩家数
	System::Memory CS2_MEM = { "cs2.exe" }; HWND CS2_HWND = CS2_MEM.Get_ProcessHWND(); uintptr_t Module_client, Module_match;//内存库初始化,模块地址
	vector<short> Global_ValidClassID = { 0 }; BOOL Global_IsShowWindow = false, Global_TeamCheck = false, Global_LowSensitivity = false; Variable::Vector4 Global_WindowResolution = Window::Get_WindowResolution(CS2_HWND);//有效实体,窗口显示,团队过滤,绕过反作弊,低灵敏度,窗口分辨率
	void ExecuteCommand(string Command) noexcept//发送指令到CS控制台
	{
		//COPYDATASTRUCT m_cData; m_cData.cbData = strlen(Command.c_str()) + 1; m_cData.dwData = 0; m_cData.lpData = (void*)Command.c_str(); SendMessage(CS2_HWND, WM_COPYDATA, 0, (LPARAM)&m_cData);//CSGO发送命令
		//-----------------------------------------------------------------------------------
		if (Command == "+jump")System::Key_Con_HWND(CS2_HWND, VK_SPACE, true);
		else if (Command == "-jump")System::Key_Con_HWND(CS2_HWND, VK_SPACE, false);
		else if (Command == "+duck")System::Key_Con_HWND(CS2_HWND, VK_CONTROL, true);
		else if (Command == "-duck")System::Key_Con_HWND(CS2_HWND, VK_CONTROL, false);
		else if (Command == "+sprint")System::Key_Con_HWND(CS2_HWND, VK_SHIFT, true);
		else if (Command == "-sprint")System::Key_Con_HWND(CS2_HWND, VK_SHIFT, false);
		else if (Command == "+lookatweapon")System::Key_Con_HWND(CS2_HWND, 'F', true);
		else if (Command == "-lookatweapon")System::Key_Con_HWND(CS2_HWND, 'F', false);
		else if (Command == "+use")System::Key_Con_HWND(CS2_HWND, 'E', true);
		else if (Command == "-use")System::Key_Con_HWND(CS2_HWND, 'E', false);
		else if (Command == "+reload")System::Key_Con_HWND(CS2_HWND, 'R', true);
		else if (Command == "-reload")System::Key_Con_HWND(CS2_HWND, 'R', false);
		else if (Command == "drop")System::Key_Click_HWND(CS2_HWND, 'G', true);
		else if (Command == "+attack")mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		else if (Command == "-attack")mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		else if (Command == "+attack2")mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		else if (Command == "-attack2")mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
		else if (Command == "+forward")System::Key_Con_HWND(CS2_HWND, 'W', true);
		else if (Command == "-forward")System::Key_Con_HWND(CS2_HWND, 'W', false);
		else if (Command == "+back")System::Key_Con_HWND(CS2_HWND, 'S', true);
		else if (Command == "-back")System::Key_Con_HWND(CS2_HWND, 'S', false);
		else if (Command == "+left")System::Key_Con_HWND(CS2_HWND, 'A', true);
		else if (Command == "-left")System::Key_Con_HWND(CS2_HWND, 'A', false);
		else if (Command == "+right")System::Key_Con_HWND(CS2_HWND, 'D', true);
		else if (Command == "-right")System::Key_Con_HWND(CS2_HWND, 'D', false);
		else if (Command == "slot1")System::Key_Click_HWND(CS2_HWND, '1', true);
		else if (Command == "slot2")System::Key_Click_HWND(CS2_HWND, '2', true);
		else if (Command == "slot3")System::Key_Click_HWND(CS2_HWND, '3', true);
		else if (Command == "slot4")System::Key_Click_HWND(CS2_HWND, '4', true);
		else if (Command == "slot5")System::Key_Click_HWND(CS2_HWND, '5', true);
		else if (Command == "slot6")System::Key_Click_HWND(CS2_HWND, '6', true);
		else if (Command == "slot7")System::Key_Click_HWND(CS2_HWND, '7', true);
		else if (Command == "slot8")System::Key_Click_HWND(CS2_HWND, '8', true);
		else if (Command == "slot9")System::Key_Click_HWND(CS2_HWND, '9', true);
		else if (Command == "+showscores")System::Key_Con_HWND(CS2_HWND, VK_TAB, true);
		else if (Command == "-showscores")System::Key_Con_HWND(CS2_HWND, VK_TAB, false);
		else if (Command == "quit")Window::Kill_Window(CS2_HWND);
		else if (Command.substr(0, 4) == "say " && Global_IsShowWindow)
		{
			Command.erase(0, 4);
			System::Key_Click_HWND(CS2_HWND, 'Y', true);
			System::Send_String(Command); Sleep(10);
			System::Key_Click_HWND(CS2_HWND, VK_RETURN, true);
		}
		else if (Command == "removeinput")for (int KeyID = 0; KeyID <= 255; ++KeyID)System::Key_Con_HWND(CS2_HWND, KeyID, false);
		else if (Command == "autosetting")
		{
			System::Key_Click_HWND(CS2_HWND, 192, true);
			System::Send_String("sv_cheats 1;bot_kick;bot_stop 1;bot_dont_shoot 1;bot_add;bot_add;bot_add;bot_add;bot_add;bot_add;mp_startmoney 99999;mp_buy_anywhere 1;sv_infinite_ammo 1;mp_restartgame 1;"); Sleep(10);
			System::Key_Click_HWND(CS2_HWND, VK_RETURN, true);
			System::Key_Click_HWND(CS2_HWND, 192, true);
		}
		//-----------------------------------------------------------------------------------
	}
	void MoveAngle(int X, int Y, float Smooth = 2.5) noexcept//通过移动鼠标移动视角
	{
		if (Global_LowSensitivity) { X /= 3; Y /= 3; }//修正高DPI移动过大问题
		if (!X && !Y)return;//无移动直接返回
		mouse_event(MOUSEEVENTF_MOVE, X, Y, 0, 0);//模拟鼠标移动
	}
	namespace CS_Offsets//CS偏移量 (游戏更新时需要同时更新 https://github.com/a2x/cs2-dumper.git)
	{
		string Offsets_Date = "Default[0000-00-00 00:00]";
		uintptr_t dwLocalPlayerController = 0x0;
		uintptr_t dwLocalPlayerPawn = 0x0;
		uintptr_t dwEntityList = 0x0;
		uintptr_t dwViewAngles = 0x0;
		uintptr_t dwViewMatrix = 0x0;
		uintptr_t dwMapName = 0x0;//dwGameTypes + 0x120
		uintptr_t dwPlantedC4 = 0x0;
		uintptr_t m_hPlayerPawn = 0x0;
		uintptr_t m_steamID = 0x0;
		uintptr_t m_iTeamNum = 0x0;
		uintptr_t m_ArmorValue = 0x0;
		uintptr_t m_iHealth = 0x0;
		uintptr_t m_iIDEntIndex = 0x0;
		uintptr_t m_fFlags = 0x0;
		uintptr_t m_iShotsFired = 0x0;
		uintptr_t m_vecVelocity = 0x0;
		uintptr_t m_bSpotted = 0x0;//m_entitySpottedState + m_bSpotted
		uintptr_t m_bIsScoped = 0x0;
		uintptr_t m_pClippingWeapon = 0x0;
		uintptr_t m_pGameSceneNode = 0x0;
		uintptr_t m_vecOrigin = 0x0;
		uintptr_t m_aimPunchCache = 0x0;
		uintptr_t m_vecViewOffset = 0x0;
		uintptr_t m_dwBoneMatrix = 0x0;//m_modelState + 0x80
		uintptr_t m_iszPlayerName = 0x0;
		uintptr_t m_pActionTrackingServices = 0x0;
		uintptr_t m_iNumRoundKills = 0x0;
		uintptr_t m_flTotalRoundDamageDealt = 0x0;
		uintptr_t m_iItemDefinitionIndex = 0x0;//m_AttributeManager + m_Item + m_iItemDefinitionIndex
		uintptr_t m_angEyeAngles = 0x0;
		uintptr_t m_nBombSite = 0x0;
		uintptr_t m_flTimerLength = 0x0;
	}
	namespace Base//基础内存函数
	{
		uintptr_t EntityList() noexcept { return CS2_MEM.Read<uintptr_t>(Module_client + CS_Offsets::dwEntityList); }//实体列表
		uintptr_t Convert(uintptr_t EntityList, uintptr_t Player) noexcept { return CS2_MEM.Read<uintptr_t>(CS2_MEM.Read<uintptr_t>(EntityList + 8 * ((Player & 0x7FFF) >> 9) + 16) + 112 * (Player & 0x1FF)); }//各种转换
		class PlayerPawn//玩家Pawn内存类
		{
			//if (bSpottedByMask & (uint32_t(1) << (LocalPlayerControllerIndex - 1)))
		private: uintptr_t m_PlayerPawn = 0; public: PlayerPawn(uintptr_t dwPlayerPawn) noexcept { m_PlayerPawn = dwPlayerPawn; };
			   BOOL operator==(const PlayerPawn other) const noexcept { return m_PlayerPawn == other.Pawn(); }//等于运算符
			   uintptr_t Pawn() const noexcept { return m_PlayerPawn; }//人物地址
			   short Health() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_iHealth); }//人物血量
			   short Armor() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_ArmorValue); }//人物护甲
			   short TeamNumber() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_iTeamNum); }//人物队伍
			   short IDEntIndex() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_iIDEntIndex); }//人物瞄准的实体ID
			   short Flags() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_fFlags); }//人物状态 (跳跃,蹲下等动作)
			   short ShotsFired() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_iShotsFired); }//人物开枪子弹数量
			   short Spotted() const noexcept { return CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_bSpotted); }//人物发现状态
			   short Scoped() const noexcept { const auto Scoped = CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_bIsScoped); if (Scoped == 65536)return 0; else return Scoped; }//人物狙击枪开镜
			   short ActiveWeapon(BOOL Type = 0) const noexcept//人物手持武器(类型,ID)
			   {
				   const auto ClippingWeapon = CS2_MEM.Read<short>(CS2_MEM.Read<uintptr_t>(m_PlayerPawn + CS_Offsets::m_pClippingWeapon) + CS_Offsets::m_iItemDefinitionIndex);
				   if (Type)//武器类型 0:无效 1:手枪 2:连射枪 3:狙击枪 4:霰弹枪
				   {
					   switch (ClippingWeapon)
					   {
					   case 1: return 1;
					   case 2: return 1;
					   case 3: return 1;
					   case 4: return 1;
					   case 7: return 2;
					   case 8: return 2;
					   case 9: return 3;
					   case 10: return 2;
					   case 11: return 3;
					   case 13: return 2;
					   case 14: return 2;
					   case 16: return 2;
					   case 17: return 2;
					   case 19: return 2;
					   case 23: return 2;
					   case 24: return 2;
					   case 25: return 4;
					   case 26: return 2;
					   case 27: return 4;
					   case 28: return 2;
					   case 29: return 4;
					   case 30: return 1;
					   case 32: return 1;
					   case 33: return 2;
					   case 34: return 2;
					   case 35: return 4;
					   case 36: return 1;
					   case 38: return 3;
					   case 39: return 2;
					   case 40: return 3;
					   case 60: return 2;
					   case 61: return 1;
					   case 63: return 1;
					   case 64: return 1;
					   default: return 0;
					   }
				   }
				   else return ClippingWeapon;
			   }
			   short ActiveWeaponDamage() const noexcept//人物手持武器命中胸部时预计伤害值 (如果目标只剩些返回值那些血量那么打身体一击毙命)
			   {
				   const auto ClippingWeapon = CS2_MEM.Read<short>(CS2_MEM.Read<uintptr_t>(m_PlayerPawn + CS_Offsets::m_pClippingWeapon) + CS_Offsets::m_iItemDefinitionIndex);
				   switch (ClippingWeapon)
				   {
				   case 1: return 35;
				   case 2: return 12;
				   case 3: return 20;
				   case 4: return 10;
				   case 7: return 25;
				   case 8: return 21;
				   case 9: return 80;
				   case 10: return 16;
				   case 11: return 50;
				   case 13: return 20;
				   case 14: return 23;
				   case 16: return 22;
				   case 17: return 11;
				   case 19: return 11;
				   case 23: return 11;
				   case 24: return 15;
				   case 25: return 30;
				   case 26: return 11;
				   case 27: return 30;
				   case 28: return 15;
				   case 29: return 15;
				   case 30: return 16;
				   case 32: return 13;
				   case 33: return 11;
				   case 34: return 12;
				   case 35: return 30;
				   case 36: return 15;
				   case 38: return 50;
				   case 39: return 20;
				   case 40: return 60;
				   case 60: return 17;
				   case 61: return 15;
				   case 63: return 6;
				   case 64: return 30;
				   default: return 20;
				   }
			   }
			   float MoveSpeed() const noexcept { const auto Velocity = CS2_MEM.Read<Variable::Vector3>(m_PlayerPawn + CS_Offsets::m_vecVelocity); return hypot(Velocity.x, Velocity.y); }//人物移动速度
			   string ActiveWeaponName(BOOL Use_WeaponBase = false, uintptr_t WeaponBase = 0, BOOL ReturnID = false) const noexcept//人物手持武器名称
			   {
				   auto ClippingWeapon = CS2_MEM.Read<short>(CS2_MEM.Read<uintptr_t>(m_PlayerPawn + CS_Offsets::m_pClippingWeapon) + CS_Offsets::m_iItemDefinitionIndex);
				   if (Use_WeaponBase)ClippingWeapon = CS2_MEM.Read<short>(WeaponBase + CS_Offsets::m_iItemDefinitionIndex);
				   if (ReturnID)return to_string(ClippingWeapon);
				   if (ClippingWeapon >= 500 && ClippingWeapon <= 600)return "KNIFE";
				   switch (ClippingWeapon)
				   {
				   case 1: return "DEAGLE";
				   case 2: return "DUAL BERETTAS";
				   case 3: return "FN57";
				   case 4: return "GLOCK-18";
				   case 7: return "AK-47";
				   case 8: return "AUG";
				   case 9: return "AWP";
				   case 10: return "FAMAS";
				   case 11: return "G3SG1";
				   case 13: return "GALILAR";
				   case 14: return "M249";
				   case 16: return "M4A4";
				   case 17: return "MAC-10";
				   case 19: return "P90";
				   case 23: return "MP5";
				   case 24: return "UMP-45";
				   case 25: return "XM1014";
				   case 26: return "BIZON";
				   case 27: return "MAG-7";
				   case 28: return "NEGEV";
				   case 29: return "SAWED";
				   case 30: return "TEC-9";
				   case 31: return "TASER";
				   case 32: return "P2000";
				   case 33: return "MP7";
				   case 34: return "MP9";
				   case 35: return "NOVA";
				   case 36: return "P250";
				   case 38: return "SCAR-20";
				   case 39: return "SG553";
				   case 40: return "SSG08";
				   case 42: return "KNIFE";
				   case 43: return "FLASH";
				   case 44: return "GRENADE";
				   case 45: return "SMOKE";
				   case 46: return "MOLOTOV";
				   case 47: return "DECOY";
				   case 48: return "INCGRNNADE";
				   case 49: return "BOMB";
				   case 59: return "KNIFE";
				   case 60: return "M4A1-S";
				   case 61: return "USP-S";
				   case 63: return "CZ75";
				   case 64: return "REVOLVER";
				   default: return "NONE";
				   }
			   }
			   string EntityName() const noexcept { return CS2_MEM.Read_str(CS2_MEM.Read<uintptr_t>(CS2_MEM.Read<uintptr_t>(m_PlayerPawn + 0x10) + 0x20)); }//实体名称(非人物)
			   Variable::Vector3 Velocity() const noexcept { return CS2_MEM.Read<Variable::Vector3>(m_PlayerPawn + CS_Offsets::m_vecVelocity); }//人物单方向移动速度
			   Variable::Vector3 Origin() const noexcept { return CS2_MEM.Read<Variable::Vector3>(CS2_MEM.Read<uintptr_t>(m_PlayerPawn + CS_Offsets::m_pGameSceneNode) + CS_Offsets::m_vecOrigin); }//人物世界坐标
			   Variable::Vector3 AimPunchAngle() const noexcept//人物手持武器后坐力
			   {
				   struct UtlVec { DWORD64 count; DWORD64 data; }; const auto PunchCache = CS2_MEM.Read<UtlVec>(m_PlayerPawn + CS_Offsets::m_aimPunchCache);//后座力缓存
				   static Variable::Vector3 PunchAngle = { 0,0,0 }; if (PunchCache.count > 0 && PunchCache.count < 0xFFFF)PunchAngle = CS2_MEM.Read<Variable::Vector3>(PunchCache.data + (PunchCache.count - 1) * sizeof(Variable::Vector3)); return PunchAngle;
			   }
			   Variable::Vector3 ViewOffset() const noexcept { return CS2_MEM.Read<Variable::Vector3>(m_PlayerPawn + CS_Offsets::m_vecViewOffset); }//人物朝向偏移
			   Variable::Vector3 BonePos(short Bone_ID = 3) const noexcept//人物骨骼坐标
			   {
				   const auto BoneMatrix = CS2_MEM.Read<uintptr_t>(CS2_MEM.Read<uintptr_t>(m_PlayerPawn + CS_Offsets::m_pGameSceneNode) + CS_Offsets::m_dwBoneMatrix);
				   return CS2_MEM.Read<Variable::Vector3>(BoneMatrix + Bone_ID * 0x20);
			   }
			   Variable::Vector3 ViewAngles() const noexcept { return CS2_MEM.Read<Variable::Vector3>(m_PlayerPawn + CS_Offsets::m_angEyeAngles); }//人物朝向角度
			   PlayerPawn IDEntIndex_Pawn() const noexcept { return Convert(EntityList(), CS2_MEM.Read<short>(m_PlayerPawn + CS_Offsets::m_iIDEntIndex)); }//瞄准的实体Pawn
			   template<class ClassName>ClassName Read(uintptr_t offset) const noexcept { return CS2_MEM.Read<ClassName>(m_PlayerPawn + offset); }//读取特定Pawn地址
			   //template<class ClassName>void Write(uintptr_t offset, ClassName value) const noexcept { CS2_MEM.Write<ClassName>(m_PlayerPawn + offset, value); }//写入特定Pawn地址
		};
		PlayerPawn LocalPlayer() noexcept { return CS2_MEM.Read<uintptr_t>(Module_client + CS_Offsets::dwLocalPlayerPawn); }//本地人物
		uintptr_t LocalPlayerController() noexcept { return CS2_MEM.Read<uintptr_t>(Module_client + CS_Offsets::dwLocalPlayerController); }//本地人物控制器
		Variable::Vector3 ViewAngles() noexcept { return CS2_MEM.Read<Variable::Vector3>(Module_client + CS_Offsets::dwViewAngles); }//本地人物视角坐标
		Variable::view_matrix_t ViewMatrix() noexcept { return CS2_MEM.Read<Variable::view_matrix_t>(Module_client + CS_Offsets::dwViewMatrix); }//本地人物视觉矩阵(用于制作ESP)
		string MapName(BOOL Original = false) noexcept//本地地图名
		{
			static const vector<string> MapList = { "train","anubis","ancient","nuke","mirage","inferno","dust2","overpass","vertigo","office","aim_botz","<empty>" };
			const auto Map_Name = CS2_MEM.Read_str(CS2_MEM.Read<uintptr_t>(Module_match + CS_Offsets::dwMapName)); if (Original)return Map_Name;//返回原始地图名
			for (int m = 0; m < MapList.size(); ++m)if (Variable::String_Find(Map_Name, MapList[m]))return MapList[m];
			return Map_Name;
		}
		struct Bomb { BOOL IsPlanted, Site; float Timer_Max, Timer; Variable::Vector3 Origin; };//炸弹信息结构体
		Bomb BombInfo() noexcept//炸弹信息
		{
			Bomb C4_Info = {}; static auto BombTickStart = steady_clock::now();
			const auto C4_Addr = CS2_MEM.Read<uintptr_t>(CS2_MEM.Read<uintptr_t>(Module_client + CS_Offsets::dwPlantedC4));
			C4_Info.IsPlanted = CS2_MEM.Read<BOOL>(Module_client + CS_Offsets::dwPlantedC4 - 8);//炸弹是否被安放
			if (C4_Info.IsPlanted)//炸弹被安放时读取信息
			{
				C4_Info.Site = CS2_MEM.Read<BOOL>(C4_Addr + CS_Offsets::m_nBombSite);//炸弹位置 (A点或B点)
				C4_Info.Timer_Max = CS2_MEM.Read<float>(C4_Addr + CS_Offsets::m_flTimerLength);//炸弹总时长
				C4_Info.Timer = C4_Info.Timer_Max - duration<float, milli>(steady_clock::now() - BombTickStart).count() / 1000;//炸弹爆炸时间
				C4_Info.Origin = CS2_MEM.Read<Variable::Vector3>(CS2_MEM.Read<uintptr_t>(C4_Addr + CS_Offsets::m_pGameSceneNode) + CS_Offsets::m_vecOrigin);//炸弹世界坐标
			}
			else BombTickStart = steady_clock::now();
			if (C4_Info.Timer < 0) { C4_Info.Timer = 0; C4_Info.IsPlanted = false; }
			return C4_Info;
		}
	}
	Base::PlayerPawn Global_LocalPlayer = 0;//本地人物Pawn地址
	namespace Advanced//进阶内存扩展函数
	{
		Base::PlayerPawn Traverse_Player(short i, BOOL ReturnPlayerController = false) noexcept//遍历人物Pawn
		{
			const auto Entitylist = Base::EntityList(), PlayerController = Base::Convert(Entitylist, i);
			if (ReturnPlayerController)return PlayerController; return Base::Convert(Entitylist, CS2_MEM.Read<uint32_t>(PlayerController + CS_Offsets::m_hPlayerPawn));
		}
		string LocalPlayer_Name() noexcept//本地人物名称
		{
			const auto LocalPlayerName = CS2_MEM.Read_str(Base::LocalPlayerController() + CS_Offsets::m_iszPlayerName);
			static string ReturnValue = "None"; if (LocalPlayerName != "")ReturnValue = LocalPlayerName; return ReturnValue;
		}
		string Player_Name(short i) noexcept//通过ClassID获取名称
		{
			const auto PlayerController = Base::Convert(Base::EntityList(), i); if (!PlayerController)return "None";
			return CS2_MEM.Read_str(PlayerController + CS_Offsets::m_iszPlayerName);
		}
		uint64_t Player_SteamID(short i, BOOL Longer = false) noexcept//通过ClassID获取SteamID
		{
			const auto PlayerController = Base::Convert(Base::EntityList(), i); if (!PlayerController)return 0;
			if (Longer)return CS2_MEM.Read<uint64_t>(PlayerController + CS_Offsets::m_steamID);//个人资料ID
			else return CS2_MEM.Read<uint32_t>(PlayerController + CS_Offsets::m_steamID);//好友ID
		}
		BOOL Check_Enemy(Base::PlayerPawn PlayerPawn) noexcept { return (PlayerPawn.Pawn() != Global_LocalPlayer.Pawn() && PlayerPawn.Health() && (!Global_TeamCheck || Global_LocalPlayer.TeamNumber() != PlayerPawn.TeamNumber())); }//判断人物是否是敌人
		BOOL Stop_Move(short TriggerValue = 65, BOOL Movement = true) noexcept//急停
		{
			const auto LocalVel = Global_LocalPlayer.Velocity();
			if (hypot(LocalVel.x, LocalVel.y) <= TriggerValue)return true;//当精准则返回真
			if (Movement)//停止移动 反之只返回是否精准
			{
				const float LocalYaw = Base::ViewAngles().y, X = (LocalVel.x * cos(LocalYaw / 180 * 3.1415926) + LocalVel.y * sin(LocalYaw / 180 * 3.1415926)), Y = (LocalVel.y * cos(LocalYaw / 180 * 3.1415926) - LocalVel.x * sin(LocalYaw / 180 * 3.1415926));
				ExecuteCommand("-back"); ExecuteCommand("-forward"); ExecuteCommand("-right"); ExecuteCommand("-left"); Sleep(1);
				if (X > 30) { ExecuteCommand("+back"); Sleep(1); ExecuteCommand("-back"); }
				else if (X < -30) { ExecuteCommand("+forward"); Sleep(1); ExecuteCommand("-forward"); }
				if (Y > 30) { ExecuteCommand("+right"); Sleep(1); ExecuteCommand("-right"); }
				else if (Y < -30) { ExecuteCommand("+left"); Sleep(1); ExecuteCommand("-left"); }
			}
			return false;
		}

		string BoneID_PartName(short BoneID = 0) noexcept//骨骼ID转换部位名称
		{
			switch (BoneID)
			{
			case 0: return "Crotch";
			case 1: return "Crotch";
			case 2: return "Waist";
			case 3: return "LowerChest";
			case 4: return "UpperChest";
			case 5: return "Neck";
			case 6: return "Head";
			}
			return "None";//防止崩溃
		}
	}
	void ReLoad(BOOL Timeout = false) noexcept//刷新CS2进程地址和模块地址和有效实体
	{
		static bool InitializationState = false; if (!InitializationState) { InitializationState = true; Timeout = true; }//第一次运行时强制刷新
		if (System::Sleep_Tick<class CS2_SDK_Memory_Reload>(1337, Timeout) || Timeout)
		{
			CS2_MEM = { "cs2.exe" }; CS2_HWND = CS2_MEM.Get_ProcessHWND(); Module_client = CS2_MEM.Get_Module("client.dll"), Module_match = CS2_MEM.Get_Module("matchmaking.dll"); Global_LocalPlayer = Base::LocalPlayer();
			if (System::Sleep_Tick<class CS2_SDK_Offsets_Reload>(30000, Timeout) || Timeout)//自动更新偏移量延迟 (减少流量使用)
			{
					System::URL_Read URL_OFFSETS = { "Cache_CS_Offsets" };
					static const vector<string> Offsets_URL_List = {//偏移量文件链接列表
						"https://github.com/Coslly/Coslly/blob/main/RensenDepend/Offsets.ofs?raw=true",//Coslly个人主页文件
					};
					if (URL_OFFSETS.Store_Mem_s(Offsets_URL_List))//自动更新偏移量 Github更新有十分钟延迟
					{
						CS_Offsets::Offsets_Date = URL_OFFSETS.Read_Var("Offsets_Date");//偏移更新日期
						CS_Offsets::Offsets_Date = "Cloud[" + CS_Offsets::Offsets_Date + "]";//加上所属类型和括号
						CS_Offsets::dwLocalPlayerController = Variable::string_uint_(URL_OFFSETS.Read_Var("dwLocalPlayerController"));
						CS_Offsets::dwLocalPlayerPawn = Variable::string_uint_(URL_OFFSETS.Read_Var("dwLocalPlayerPawn"));
						CS_Offsets::dwEntityList = Variable::string_uint_(URL_OFFSETS.Read_Var("dwEntityList"));
						CS_Offsets::dwViewAngles = Variable::string_uint_(URL_OFFSETS.Read_Var("dwViewAngles"));
						CS_Offsets::dwViewMatrix = Variable::string_uint_(URL_OFFSETS.Read_Var("dwViewMatrix"));
						CS_Offsets::dwMapName = Variable::string_uint_(URL_OFFSETS.Read_Var("dwMapName"));
						CS_Offsets::dwPlantedC4 = Variable::string_uint_(URL_OFFSETS.Read_Var("dwPlantedC4"));
						CS_Offsets::m_hPlayerPawn = Variable::string_uint_(URL_OFFSETS.Read_Var("m_hPlayerPawn"));
						CS_Offsets::m_steamID = Variable::string_uint_(URL_OFFSETS.Read_Var("m_steamID"));
						CS_Offsets::m_iTeamNum = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iTeamNum"));
						CS_Offsets::m_ArmorValue = Variable::string_uint_(URL_OFFSETS.Read_Var("m_ArmorValue"));
						CS_Offsets::m_iHealth = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iHealth"));
						CS_Offsets::m_iIDEntIndex = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iIDEntIndex"));
						CS_Offsets::m_fFlags = Variable::string_uint_(URL_OFFSETS.Read_Var("m_fFlags"));
						CS_Offsets::m_iShotsFired = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iShotsFired"));
						CS_Offsets::m_vecVelocity = Variable::string_uint_(URL_OFFSETS.Read_Var("m_vecVelocity"));
						CS_Offsets::m_bSpotted = Variable::string_uint_(URL_OFFSETS.Read_Var("m_bSpotted"));
						CS_Offsets::m_bIsScoped = Variable::string_uint_(URL_OFFSETS.Read_Var("m_bIsScoped"));
						CS_Offsets::m_pClippingWeapon = Variable::string_uint_(URL_OFFSETS.Read_Var("m_pClippingWeapon"));
						CS_Offsets::m_pGameSceneNode = Variable::string_uint_(URL_OFFSETS.Read_Var("m_pGameSceneNode"));
						CS_Offsets::m_vecOrigin = Variable::string_uint_(URL_OFFSETS.Read_Var("m_vecOrigin"));
						CS_Offsets::m_aimPunchCache = Variable::string_uint_(URL_OFFSETS.Read_Var("m_aimPunchCache"));
						CS_Offsets::m_vecViewOffset = Variable::string_uint_(URL_OFFSETS.Read_Var("m_vecViewOffset"));
						CS_Offsets::m_dwBoneMatrix = Variable::string_uint_(URL_OFFSETS.Read_Var("m_dwBoneMatrix"));
						CS_Offsets::m_iszPlayerName = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iszPlayerName"));
						CS_Offsets::m_pActionTrackingServices = Variable::string_uint_(URL_OFFSETS.Read_Var("m_pActionTrackingServices"));
						CS_Offsets::m_iNumRoundKills = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iNumRoundKills"));
						CS_Offsets::m_flTotalRoundDamageDealt = Variable::string_uint_(URL_OFFSETS.Read_Var("m_flTotalRoundDamageDealt"));
						CS_Offsets::m_iItemDefinitionIndex = Variable::string_uint_(URL_OFFSETS.Read_Var("m_iItemDefinitionIndex"));
						CS_Offsets::m_angEyeAngles = Variable::string_uint_(URL_OFFSETS.Read_Var("m_angEyeAngles"));
						CS_Offsets::m_nBombSite = Variable::string_uint_(URL_OFFSETS.Read_Var("m_nBombSite"));
						CS_Offsets::m_flTimerLength = Variable::string_uint_(URL_OFFSETS.Read_Var("m_flTimerLength"));
					}
			}
			if (CS2_HWND)//CS运行时遍历刷新有效实体ID
			{
				Global_ValidClassID = {};//遍历前刷新有效实体ID
				for (short i = 0; i <= MAX_PLAYER; ++i)
				{
					const auto PlayerPawn = Advanced::Traverse_Player(i);//通过人物ID转换人物Pawn
					if (!PlayerPawn.Health() || (!PlayerPawn.Origin().x && !PlayerPawn.Origin().y) || !PlayerPawn.ActiveWeapon())continue;//多点检测 (过滤掉死亡或是观战的玩家)
					Global_ValidClassID.push_back(i);//将人物ID写入有效实体变量内
				}
			}
		}
		Global_IsShowWindow = CS2_HWND == GetForegroundWindow();//刷新是否是最前端窗口
		if (CS2_HWND)//刷新窗口分辨率
		{
			const auto ScrRes = Window::Get_Resolution(); auto WindowResolution = Window::Get_WindowResolution(CS2_HWND);
			if (WindowResolution.r == ScrRes.x && WindowResolution.g == ScrRes.y) { WindowResolution.b = 0; WindowResolution.a = 0; }//修复绘制窗口偏移
			Global_WindowResolution = WindowResolution;
		}
		else Global_WindowResolution = { 0,0,0,0 };
	}
}
using namespace CS_SDK;