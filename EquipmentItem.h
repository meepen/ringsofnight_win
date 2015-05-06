#ifndef EQUIPMENTITEM_H
#define EQUIPMENTITEM_H
#include "commands.h"

namespace RON
{

	enum EQUIPMENTITEM_TYPES
	{
		WEAPON = 1,
		ARMOR, 
		STORAGE,
		HARVESTER, 
		ENGINE, 
		COMPUTER, 
		SPECIAL
	};

	class EquipmentItem
	{
	public:
		void read(char type, CharBuf &buf)
		{
			bool success;
			this->type = type;
			buf.read(&rarity, &success);
			buf.read(&durability, &success);
			buf.read(&noDrop, &success);
			if (noDrop == -128)
				noDrop = 0;
			else
				noDrop = 1;
		}

		virtual CharBuf write(void)
		{
			CharBuf ret = CharBuf();
			ret.append(type);
			ret.append(rarity);
			ret.append(durability);
			if (noDrop)
				ret.append((char)127);
			else
				ret.append((char)-128);
			return ret;
		}

		virtual const char *getName(void)
		{
			return "Unknown";
		}

		inline static EquipmentItem *instantiate(CharBuf &buf);


	public:
		char type;
		char rarity;
		char durability;
		char noDrop;

		EquipmentItem *next;
	};

	class WeaponItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		WeaponItem(CharBuf &buf)
		{
			bool success;
			super::read(WEAPON, buf);
			buf.read(&_class, &success);
			buf.read(&baseEP, &success);
		}
		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			ret.append(baseEP);
			return ret;
		}

		int getShootSpeedMS(void)
		{
			switch (_class)
			{
			case 19:
			case 38:
			case 39:
			case 58:
			case 59:
			default:
				return 1073741823;
			case 1:
				return 2000;
			case 20:
				return 2100;
			case 40:
				return 2200;
			case 60:
				return 2300;
			case 2:
				return 2400;
			case 21:
				return 2500;
			case 41:
				return 2600;
			case 61:
				return 2700;
			case 3:
				return 2800;
			case 22:
				return 2900;
			case 42:
				return 3000;
			case 62:
				return 3100;
			case 4:
				return 3200;
			case 23:
				return 3300;
			case 43:
				return 3400;
			case 63:
				return 3500;
			case 5:
				return 3600;
			case 24:
				return 3700;
			case 44:
				return 3800;
			case 64:
				return 3900;
			case 6:
				return 4000;
			case 25:
				return 4100;
			case 45:
				return 4200;
			case 65:
				return 4300;
			case 7:
				return 4400;
			case 26:
				return 4500;
			case 46:
				return 4600;
			case 66:
				return 4700;
			case 8:
				return 4800;
			case 27:
				return 4900;
			case 47:
				return 5000;
			case 67:
				return 5100;
			case 9:
				return 5200;
			case 28:
				return 5300;
			case 48:
				return 5400;
			case 68:
				return 5500;
			case 10:
				return 5600;
			case 29:
				return 5700;
			case 49:
				return 5800;
			case 69:
				return 5900;
			case 11:
				return 6000;
			case 30:
				return 6100;
			case 50:
				return 6200;
			case 70:
				return 6300;
			case 12:
				return 6400;
			case 31:
				return 6500;
			case 51:
				return 6600;
			case 71:
				return 6700;
			case 13:
				return 6800;
			case 32:
				return 6900;
			case 52:
				return 7000;
			case 72:
				return 7100;
			case 14:
				return 7200;
			case 33:
				return 7300;
			case 53:
				return 7400;
			case 73:
				return 7500;
			case 15:
				return 7600;
			case 34:
				return 7700;
			case 54:
				return 7800;
			case 74:
				return 7900;
			case 16:
				return 8000;
			case 35:
				return 8100;
			case 55:
				return 8200;
			case 75:
				return 8300;
			case 17:
				return 8400;
			case 36:
				return 8500;
			case 56:
				return 8600;
			case 76:
				return 8700;
			case 18:
				return 8800;
			case 37:
				return 8900;
			case 57:
				return 9000;
			case 77:
				return 10000;
			}
		}

		const char *getName(void)
		{
			switch (_class)
			{
			case 19:
			case 38:
			case 39:
			case 58:
			case 59:
			default:
				return "Unknown?";
			case 1:
				return "Mutilator";
			case 2:
				return "Starshatter";
			case 3:
				return "Striker";
			case 4:
				return "Exterminator";
			case 5:
				return "Voidblaster";
			case 6:
				return "Ravager";
			case 7:
				return "Brutalizer";
			case 8:
				return "Vaporizer";
			case 9:
				return "Desolator";
			case 10:
				return "Atomizer";
			case 11:
				return "Corruptor";
			case 12:
				return "Mindslayer";
			case 13:
				return "Riftbreaker";
			case 14:
				return "Soultaker";
			case 15:
				return "Nullcannon";
			case 16:
				return "Demolisher";
			case 17:
				return "Incinerator";
			case 18:
				return "Eradicator";
			case 20:
				return "Rapture";
			case 21:
				return "Glory";
			case 22:
				return "Oblivion";
			case 23:
				return "Horror";
			case 24:
				return "Ruin";
			case 25:
				return "Cataclysm";
			case 26:
				return "Torment";
			case 27:
				return "Smolder";
			case 28:
				return "Destruction";
			case 29:
				return "Frenzy";
			case 30:
				return "Silence";
			case 31:
				return "Exodus";
			case 32:
				return "Darkness";
			case 33:
				return "Agony";
			case 34:
				return "Prophecy";
			case 35:
				return "Radiance";
			case 36:
				return "Animus";
			case 37:
				return "Pain";
			case 40:
				return "Screamer";
			case 41:
				return "Succubus";
			case 42:
				return "Banshee";
			case 43:
				return "Basilisk";
			case 44:
				return "Harpy";
			case 45:
				return "Wyvern";
			case 46:
				return "Viper";
			case 47:
				return "Ripper";
			case 48:
				return "Penatrator";
			case 49:
				return "Serpent";
			case 50:
				return "Hydra";
			case 51:
				return "Firecat";
			case 52:
				return "Hellcannon";
			case 53:
				return "Ophidian";
			case 54:
				return "Behemoth";
			case 55:
				return "Gargoyle";
			case 56:
				return "Kraken";
			case 57:
				return "Dragon";
			case 60:
				return "Burst Cannon";
			case 61:
				return "Proton Launcher";
			case 62:
				return "Auto Cannon";
			case 63:
				return "Fusion Beam";
			case 64:
				return "Phaser";
			case 65:
				return "Mass Driver";
			case 66:
				return "Gauss Cannon";
			case 67:
				return "Meson Blaster";
			case 68:
				return "Omega Rifle";
			case 69:
				return "Leviathan";
			case 70:
				return "Accelerator";
			case 71:
				return "Rail Gun";
			case 72:
				return "Disruptor";
			case 73:
				return "Gravity Smasher";
			case 74:
				return "Pulverizer";
			case 75:
				return "Ion Cannon";
			case 76:
				return "Plasma Lance";
			case 77:
				return "Matter Inverter";
			}
		}

	public:
		char _class;
		char baseEP;
	};

	class ArmorItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		ArmorItem(CharBuf &buf)
		{
			bool success;
			super::read(ARMOR, buf);
			buf.read(&_class, &success);
			buf.read(&baseEP, &success);
		}

		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			ret.append(baseEP);
			return ret;
		}
		
		const char *getName(void)
		{
			switch (_class)
			{
			default:
				return "Unknown?";
			case 0:
				return "Titanium Hull";
			case 1:
				return "Composite Hull";
			case 2:
				return "Carbide Hull";
			case 10:
				return "Exoprene Shield";
			case 11:
				return "Cytoplast Shield";
			case 12:
				return "Holocrine Shield";
			case 20:
				return "Diamond Armor";
			case 21:
				return "Thorium Armor";
			case 22:
				return "Osmium Armor";
			case 30:
				return "Citadel Gambit";
			case 31:
				return "Ajax Gambit";
			case 32:
				return "Aegis Gambit";
			case 33:
				return "Kismet Gambit";
			}
		}

	public:
		char _class;
		char baseEP;
	};

	class StorageItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		StorageItem(CharBuf &buf)
		{
			bool success;
			super::read(STORAGE, buf);
			buf.read(&_class, &success);
			buf.read(&baseEP, &success);
		}

		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			ret.append(baseEP);
			return ret;
		}

		const char *getName(void)
		{
			switch (_class)
			{
			default:
				return "Unknown?";
			case -126:
				return "Hold-H1A";
			case -125:
				return "Hold-H2A";
			case -124:
				return "Hold-H1B";
			case -123:
				return "Hold-H2B";
			case -122:
				return "Hold-H1G";
			case -121:
				return "Hold-H2G";
			case -120:
				return "Hold-P1A";
			case -119:
				return "Hold-P2A";
			case -118:
				return "Hold-P1B";
			case -117:
				return "Hold-P2B";
			case -116:
				return "Hold-P1G";
			case -115:
				return "Hold-P2G";
			case -114:
				return "Hold-W1A";
			case -113:
				return "Hold-W2A";
			case -112:
				return "Hold-W1B";
			case -111:
				return "Hold-W2B";
			case -110:
				return "Hold-W1G";
			case -109:
				return "Hold-W2G";
			case -108:
				return "Hold-R1A";
			case -107:
				return "Hold-R2A";
			case -106:
				return "Hold-R1B";
			case -105:
				return "Hold-R2B";
			case -104:
				return "Hold-R1G";
			case -103:
				return "Hold-R2G";
			case -102:
				return "Hold-H102";
			case -101:
				return "Hold-H103";
			case -100:
				return "Hold-H104";
			case -99:
				return "Hold-H105";
			case -98:
				return "Hold-H106";
			case -97:
				return "Hold-H107";
			case -96:
				return "Hold-H108";
			case -95:
				return "Hold-H109";
			case -94:
				return "Hold-H110";
			case -93:
				return "Hold-H111";
			case -92:
				return "Hold-H112";
			case -91:
				return "Hold-H113";
			case -90:
				return "Hold-H114";
			case -89:
				return "Hold-H115";
			case -88:
				return "Hold-H116";
			case -87:
				return "Hold-H117";
			case -86:
				return "Hold-H118";
			case -85:
				return "Hold-H119";
			case -84:
				return "Hold-H120";
			case -83:
				return "Hold-H121";
			case -82:
				return "Hold-H122";
			case -81:
				return "Hold-H123";
			case -80:
				return "Hold-H124";
			case -79:
				return "Hold-H125";
			case -78:
				return "Hold-H202";
			case -77:
				return "Hold-H203";
			case -76:
				return "Hold-H204";
			case -75:
				return "Hold-H205";
			case -74:
				return "Hold-H206";
			case -73:
				return "Hold-H207";
			case -72:
				return "Hold-H208";
			case -71:
				return "Hold-H209";
			case -70:
				return "Hold-H210";
			case -69:
				return "Hold-H211";
			case -68:
				return "Hold-H212";
			case -67:
				return "Hold-H213";
			case -66:
				return "Hold-H214";
			case -65:
				return "Hold-H215";
			case -64:
				return "Hold-H216";
			case -63:
				return "Hold-H217";
			case -62:
				return "Hold-H218";
			case -61:
				return "Hold-H219";
			case -60:
				return "Hold-H220";
			case -59:
				return "Hold-H221";
			case -58:
				return "Hold-H222";
			case -57:
				return "Hold-H223";
			case -56:
				return "Hold-H224";
			case -55:
				return "Hold-H225";
			case -54:
				return "Hold-P102";
			case -53:
				return "Hold-P103";
			case -52:
				return "Hold-P104";
			case -51:
				return "Hold-P105";
			case -50:
				return "Hold-P106";
			case -49:
				return "Hold-P107";
			case -48:
				return "Hold-P108";
			case -47:
				return "Hold-P109";
			case -46:
				return "Hold-P110";
			case -45:
				return "Hold-P111";
			case -44:
				return "Hold-P112";
			case -43:
				return "Hold-P113";
			case -42:
				return "Hold-P114";
			case -41:
				return "Hold-P115";
			case -40:
				return "Hold-P116";
			case -39:
				return "Hold-P117";
			case -38:
				return "Hold-P118";
			case -37:
				return "Hold-P119";
			case -36:
				return "Hold-P120";
			case -35:
				return "Hold-P121";
			case -34:
				return "Hold-P122";
			case -33:
				return "Hold-P123";
			case -32:
				return "Hold-P124";
			case -31:
				return "Hold-P125";
			case -30:
				return "Hold-P202";
			case -29:
				return "Hold-P203";
			case -28:
				return "Hold-P204";
			case -27:
				return "Hold-P205";
			case -26:
				return "Hold-P206";
			case -25:
				return "Hold-P207";
			case -24:
				return "Hold-P208";
			case -23:
				return "Hold-P209";
			case -22:
				return "Hold-P210";
			case -21:
				return "Hold-P211";
			case -20:
				return "Hold-P212";
			case -19:
				return "Hold-P213";
			case -18:
				return "Hold-P214";
			case -17:
				return "Hold-P215";
			case -16:
				return "Hold-P216";
			case -15:
				return "Hold-P217";
			case -14:
				return "Hold-P218";
			case -13:
				return "Hold-P219";
			case -12:
				return "Hold-P220";
			case -11:
				return "Hold-P221";
			case -10:
				return "Hold-P222";
			case -9:
				return "Hold-P223";
			case -8:
				return "Hold-P224";
			case -7:
				return "Hold-P225";
			case -6:
				return "Hold-W102";
			case -5:
				return "Hold-W103";
			case -4:
				return "Hold-W104";
			case -3:
				return "Hold-W105";
			case -2:
				return "Hold-W106";
			case -1:
				return "Hold-W107";
			case 0:
				return "Hold-W108";
			case 1:
				return "Hold-W109";
			case 2:
				return "Hold-W110";
			case 3:
				return "Hold-W111";
			case 4:
				return "Hold-W112";
			case 5:
				return "Hold-W113";
			case 6:
				return "Hold-W114";
			case 7:
				return "Hold-W115";
			case 8:
				return "Hold-W116";
			case 9:
				return "Hold-W117";
			case 10:
				return "Hold-W118";
			case 11:
				return "Hold-W119";
			case 12:
				return "Hold-W120";
			case 13:
				return "Hold-W121";
			case 14:
				return "Hold-W122";
			case 15:
				return "Hold-W123";
			case 16:
				return "Hold-W124";
			case 17:
				return "Hold-W125";
			case 18:
				return "Hold-W202";
			case 19:
				return "Hold-W203";
			case 20:
				return "Hold-W204";
			case 21:
				return "Hold-W205";
			case 22:
				return "Hold-W206";
			case 23:
				return "Hold-W207";
			case 24:
				return "Hold-W208";
			case 25:
				return "Hold-W209";
			case 26:
				return "Hold-W210";
			case 27:
				return "Hold-W211";
			case 28:
				return "Hold-W212";
			case 29:
				return "Hold-W213";
			case 30:
				return "Hold-W214";
			case 31:
				return "Hold-W215";
			case 32:
				return "Hold-W216";
			case 33:
				return "Hold-W217";
			case 34:
				return "Hold-W218";
			case 35:
				return "Hold-W219";
			case 36:
				return "Hold-W220";
			case 37:
				return "Hold-W221";
			case 38:
				return "Hold-W222";
			case 39:
				return "Hold-W223";
			case 40:
				return "Hold-W224";
			case 41:
				return "Hold-W225";
			case 42:
				return "Hold-R102";
			case 43:
				return "Hold-R103";
			case 44:
				return "Hold-R104";
			case 45:
				return "Hold-R105";
			case 46:
				return "Hold-R106";
			case 47:
				return "Hold-R107";
			case 48:
				return "Hold-R108";
			case 49:
				return "Hold-R109";
			case 50:
				return "Hold-R110";
			case 51:
				return "Hold-R111";
			case 52:
				return "Hold-R112";
			case 53:
				return "Hold-R113";
			case 54:
				return "Hold-R114";
			case 55:
				return "Hold-R115";
			case 56:
				return "Hold-R116";
			case 57:
				return "Hold-R117";
			case 58:
				return "Hold-R118";
			case 59:
				return "Hold-R119";
			case 60:
				return "Hold-R120";
			case 61:
				return "Hold-R121";
			case 62:
				return "Hold-R122";
			case 63:
				return "Hold-R123";
			case 64:
				return "Hold-R124";
			case 65:
				return "Hold-R125";
			case 66:
				return "Hold-R202";
			case 67:
				return "Hold-R203";
			case 68:
				return "Hold-R204";
			case 69:
				return "Hold-R205";
			case 70:
				return "Hold-R206";
			case 71:
				return "Hold-R207";
			case 72:
				return "Hold-R208";
			case 73:
				return "Hold-R209";
			case 74:
				return "Hold-R210";
			case 75:
				return "Hold-R211";
			case 76:
				return "Hold-R212";
			case 77:
				return "Hold-R213";
			case 78:
				return "Hold-R214";
			case 79:
				return "Hold-R215";
			case 80:
				return "Hold-R216";
			case 81:
				return "Hold-R217";
			case 82:
				return "Hold-R218";
			case 83:
				return "Hold-R219";
			case 84:
				return "Hold-R220";
			case 85:
				return "Hold-R221";
			case 86:
				return "Hold-R222";
			case 87:
				return "Hold-R223";
			case 88:
				return "Hold-R224";
			case 89:
				return "Hold-R225";
			case 90:
				return "Big Crate";
			case 91:
				return "Really Big Crate";
			}
		}

	public:
		char _class;
		char baseEP;
	};

	class HarvesterItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		HarvesterItem(CharBuf &buf)
		{
			bool success;
			super::read(HARVESTER, buf);
			buf.read(&_class, &success);
		}
		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			return ret;
		}

		const char *getName(void)
		{
			switch (_class)
			{
			default:
				return "Unknown?";
			case 0:
				return "LCD-V1";
			case 11:
				return "LCD-V2";
			case 12:
				return "LCD-V3";
			case 13:
				return "LCD-V4";
			case 14:
				return "LCD-V5";
			case 15:
				return "LCD-V6";
			case 16:
				return "LCD-V7";
			case 17:
				return "LCD-V8";
			case 18:
				return "LCD-V9";
			case 19:
				return "LCD-V10";
			case 20:
				return "LCD-V11";
			case 21:
				return "LCD-V12";
			case 1:
				return "HX-V1";
			case 31:
				return "HX-V2";
			case 32:
				return "HX-V3";
			case 33:
				return "HX-V4";
			case 34:
				return "HX-V5";
			case 35:
				return "HX-V6";
			case 36:
				return "HX-V7";
			case 37:
				return "HX-V8";
			case 38:
				return "HX-V9";
			case 39:
				return "HX-V10";
			case 40:
				return "HX-V11";
			case 41:
				return "HX-V12";
			case 2:
				return "WP-V1";
			case 51:
				return "WP-V2";
			case 52:
				return "WP-V3";
			case 53:
				return "WP-V4";
			case 54:
				return "WP-V5";
			case 55:
				return "WP-V6";
			case 56:
				return "WP-V7";
			case 57:
				return "WP-V8";
			case 58:
				return "WP-V9";
			case 59:
				return "WP-V10";
			case 60:
				return "WP-V11";
			case 61:
				return "WP-V12";
			case 3:
				return "DCD-V1";
			case 71:
				return "DCD-V2";
			case 72:
				return "DCD-V3";
			case 73:
				return "DCD-V4";
			case 74:
				return "DCD-V5";
			case 75:
				return "DCD-V6";
			case 76:
				return "DCD-V7";
			case 77:
				return "DCD-V8";
			case 78:
				return "DCD-V9";
			case 79:
				return "DCD-V10";
			case 80:
				return "DCD-V11";
			case 81:
				return "DCD-V12";
			}
		}

	public:
		char _class;
	};

	class EngineItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		EngineItem(CharBuf &buf)
		{
			bool success;
			super::read(ENGINE, buf);
			buf.read(&_class, &success);
		}

		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			return ret;
		}

		const char *getName(void)
		{
			switch (_class)
			{
			default:
				return "Unknown?";
			case 0:
				return "Gravity Drive";
			case 1:
				return "Skip Drive";
			case 2:
				return "Stealth Drive";
			case 3:
				return "Neutron Drive";
			case 4:
				return "Fusion Drive";
			}
		}

	public:
		char _class;
	};

	class ComputerItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		ComputerItem(CharBuf &buf)
		{
			bool success;
			super::read(COMPUTER, buf);
			buf.read(&_class, &success);
		}

		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			return ret;
		}

		const char *getName(void)
		{
			switch (_class)
			{
			default:
				return "Unknown?";
			case 1:
				return "Sage Mk. I";
			case 2:
				return "Sage Mk. II";
			case 3:
				return "Sage Mk. III";
			case 4:
				return "Sage Mk. IV";
			case 5:
				return "Toil Mk. I";
			case 6:
				return "Toil Mk. II";
			case 7:
				return "Toil Mk. III";
			case 8:
				return "Toil Mk. IV";
			case 9:
				return "Havok Mk. I";
			case 10:
				return "Havok Mk. II";
			case 11:
				return "Havok Mk. III";
			case 12:
				return "Havok Mk. IV";
			case 13:
				return "Cabal Mk. I";
			case 14:
				return "Cabal Mk. II";
			case 15:
				return "Cabal Mk. III";
			case 16:
				return "Cabal Mk. IV";
			case 17:
				return "Agent Mk. I";
			case 18:
				return "Agent Mk. II";
			case 19:
				return "Agent Mk. III";
			case 20:
				return "Agent Mk. IV";
			case 21:
				return "Drone Mk. I";
			case 22:
				return "Drone Mk. II";
			case 23:
				return "Drone Mk. III";
			case 24:
				return "Drone Mk. IV";
			case 25:
				return "Warrior Mk. I";
			case 26:
				return "Warrior Mk. II";
			case 27:
				return "Warrior Mk. III";
			case 28:
				return "Warrior Mk. IV";
			case 29:
				return "Sage Tech I";
			case 30:
				return "Toil Tech I";
			case 31:
				return "Havok Tech I";
			case 32:
				return "Cabal Tech I";
			case 33:
				return "Agent Tech I";
			case 34:
				return "Drone Tech I";
			case 35:
				return "Warrior Tech I";
			case 36:
				return "Sage Tech II";
			case 37:
				return "Toil Tech II";
			case 38:
				return "Havok Tech II";
			case 39:
				return "Cabal Tech II";
			case 40:
				return "Agent Tech II";
			case 41:
				return "Drone Tech II";
			case 42:
				return "Warrior Tech II";
			case 43:
				return "Sage Tech III";
			case 44:
				return "Toil Tech III";
			case 45:
				return "Havok Tech III";
			case 46:
				return "Cabal Tech III";
			case 47:
				return "Agent Tech III";
			case 48:
				return "Drone Tech III";
			case 49:
				return "Warrior Tech III";
			}
		}


	public:
		char _class;
	};

	class SpecialItem : public EquipmentItem
	{
	private:
		typedef EquipmentItem super;

	public:
		SpecialItem(CharBuf &buf)
		{
			bool success;
			super::read(SPECIAL, buf);
			buf.read(&_class, &success);
		}


		CharBuf write(void)
		{
			CharBuf ret = super::write();
			ret.append(_class);
			return ret;
		}

		const char *getName(void)
		{
			switch (_class)
			{
			default:
				return "Unknown?";
			case 0:
				return "Technician";
			case 1:
				return "Prospector";
			case 2:
				return "Tank";
			case 3:
				return "Scout";
			case 4:
				return "Tractor Beam";
			case 5:
				return "Nova Device";
			case 6:
				return "Battle Ram";
			case 7:
				return "Alien Hunter";
			case 8:
				return "Deflector";
			case 9:
				return "Grappling Hook";
			case 10:
				return "Advanced Construct";
			case 11:
				return "Advanced Shields";
			case 12:
				return "Advanced Electronics";
			case 13:
				return "Advanced Munitions";
			case 14:
				return "Advanced Propulsion";
			}
		}

	public:
		char _class;
	};

	EquipmentItem *EquipmentItem::instantiate(CharBuf &buf)
	{
		char type;
		bool success;
		buf.read(&type, &success);
		switch (type)
		{
		case WEAPON:
			return new WeaponItem(buf);
		case HARVESTER:
			return new HarvesterItem(buf);
		case STORAGE:
			return new StorageItem(buf);
		case SPECIAL:
			return new SpecialItem(buf);
		case ARMOR:
			return new ArmorItem(buf);
		case COMPUTER:
			return new ComputerItem(buf);
		case ENGINE:
			return new EngineItem(buf);
		default:
			return 0;
		}
	}

}

#endif