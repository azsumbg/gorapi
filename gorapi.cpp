#include "pch.h"
#include "gorapi.h"

class EVILS :public dll::FACTORY
{
	public: 
		EVILS(float wherex, float wherey, dll::types what) :FACTORY(what, wherex, wherey)
		{
			switch (type)
			{
			case dll::types::gorilla1:
				speed = 0.8f;
				break;

			case dll::types::gorilla2:
				speed = 0.5f;
				break;

			case dll::types::gorilla3:
				speed = 0.9f;
				break;
			}
		}

		void Release() override
		{
			delete this;
		}

		int GetFrame() override
		{
			frame_delay--;
			if (frame_delay < 0)
			{
				frame++;
				
				switch (type)
				{
				case dll::types::gorilla1:
					frame_delay = 3;
					break;

				case dll::types::gorilla2:
					frame_delay = 10;
					break;

				case dll::types::gorilla3:
					frame_delay = 5;
					break;
				}

				if (frame > max_frames)frame = 0;
			}
			return frame;
		}

		int Move(float what_speed) override
		{
			float my_speed = speed + what_speed;

			x -= my_speed;
			SetEdges();
			if (ex > 0)return OK;
			return FAIL;
		}
		int Jump()override
		{
			return NaN;
		}
};

class HERO :public dll::FACTORY
{
	public:

		HERO(float wherex, float wherey) :FACTORY(dll::types::hero, wherex, wherey)
		{
			speed = 1.0f;
		}

		void Release() override
		{
			delete this;
		}

		int GetFrame() override
		{
			frame_delay--;
			if (frame_delay < 0)
			{
				frame_delay = 5;
				frame++;
				if (frame > max_frames)frame = 0;
			}
			return frame;
		}
		int Move(float what_speed) override
		{
			float my_speed = speed + what_speed;
			switch (dir)
			{
			case dll::dirs::right:
				if (ex + my_speed <= scr_width)
				{
					x += my_speed;
					SetEdges();
					return OK;
				}
				break;

			case dll::dirs::left:
				if (x - my_speed >= 0)
				{
					x -= my_speed;
					SetEdges();
					return OK;
				}
				break;
			}
			return FAIL;
		}
		int Jump() override
		{
			if (dir != dll::dirs::up && dir != dll::dirs::down)
			{
				jump_x = ex;
				jump_y = y;
				
				if (dir == dll::dirs::right || dir == dll::dirs::stop)jump_ex = jump_x + 1.5f * width;
				else if (dir == dll::dirs::left)jump_ex = jump_x - 1.5f * width;

				jump_ey = jump_y - 2.0f * height;
				jump_dir = dir;
				if (jump_dir == dll::dirs::stop)jump_dir = dll::dirs::right;
				dir = dll::dirs::up;
			}

			if (dir == dll::dirs::up)
			{
				switch (jump_dir)
				{
				case dll::dirs::right:
					if (ex + speed <= scr_width && ex + speed <= jump_ex) x += speed;
					break;

				case dll::dirs::left:
					if (x - speed >= 0 && x + speed >= jump_ex) x -= speed;
					break;
				}
				
				if (y - speed >= jump_ey && y - speed >= 50)y -= speed;
				else dir = dll::dirs::down;
				SetEdges();
				return OK;
			}

			if (dir == dll::dirs::down)
			{
				switch (jump_dir)
				{

				case dll::dirs::right:
					if (ex + speed <= scr_width && ex + speed <= jump_ex) x += speed;
					break;

				case dll::dirs::left:
					if (x - speed >= 0 && x + speed >= jump_ex) x -= speed;
					break;

				}
				if (y < jump_ey)y += speed;
				else dir = dll::dirs::stop;
				SetEdges();
				return OK;
			}

			return FAIL;
		}
};

dll::creat_ptr dll::iFactory(types what, float where_y)
{
	dll::creat_ptr ret = nullptr;

	switch (what)
	{
	case types::gorilla1:
		ret = new EVILS(scr_width, where_y, types::gorilla1);
		break;


	case types::gorilla2:
		ret = new EVILS(scr_width, where_y, types::gorilla2);
		break;

	case types::gorilla3:
		ret = new EVILS(scr_width, where_y, types::gorilla1);
		break;

	case types::hero:
		ret = new HERO(50.0f, where_y);
		break;
	}

	return ret;
}
