/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef SAMPLE_CONSOLE_H
#define SAMPLE_CONSOLE_H

#include "common/PxPhysXCommonConfig.h"
#include "RendererColor.h"
#include "RendererWindow.h"
#include "SampleAllocator.h"
#include <SampleUserInput.h>
#include <SamplePlatform.h>

	#define CONSOLE_KEY			222
	#define CONSOLE_MAX_COL		80
	#define CONSOLE_MAX_ROW		200
	#define CONSOLE_MAX_HIST	30


namespace SampleRenderer
{
	class Renderer;
}

	struct ConsoleRow : public SampleAllocateable
	{
		SampleRenderer::RendererColor	mColor;
		char							mText[CONSOLE_MAX_COL];
	};

	#define CONSOLE_MAX_COMMAND_LENGTH		48
	#define	CONSOLE_MAX_COMMAND_NB			256
	class Console;
	struct ConsoleCommand : public SampleAllocateable
	{
		char	fullcmd[CONSOLE_MAX_COMMAND_LENGTH];
		void	(*function)(Console* console, const char* text, void* user_data);
		struct ConsoleCommand* next;
	};

	enum ConsoleInputKey
	{
		CONSOLE_KEY_PRIOR,
		CONSOLE_KEY_NEXT,

		CONSOLE_KEY_UP,
		CONSOLE_KEY_DOWN,
	};

	class Console : public SampleAllocateable
	{
		public:
								Console(SampleFramework::SamplePlatform* plt);
								~Console();

				bool			render(SampleRenderer::Renderer* rnd);
				void			onKeyDown(SampleFramework::SampleUserInput::KeyCode keyCode, PxU32 param);
				void			onDigitalInputEvent(const SampleFramework::InputEvent& ie, bool val);
				void			collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents);
				void			out(const char* string);
				void			addCmd(const char* full_cmd, void (*function)(Console*, const char*, void*));

				void			clear();
				void			setPrompt(const char* text);

				bool			isActive()				const	{ return mIsActive;		}
				void			setActive(bool b)				{ mIsActive = b;		}
				void			setUserData(void* userData)		{ mUserData = userData;	}


		static void				BasicCmdexit(Console* console, const char* text, void* user_data);
		static void				BasicCmdcls(Console* console, const char* text, void* user_data);
		static void				BasicCmdSetPrompt(Console* console, const char* text, void* user_data);
		static void				BasicCmdcmdlist(Console* console, const char* text, void* user_data);
		static void				BasicCmdcmdhist(Console* console, const char* text, void* user_data);


		private:
				char			mCmdhist[CONSOLE_MAX_HIST][CONSOLE_MAX_COL];
				long			mNewcmd;
				long			mNumcmdhist;
				long			mCurcmd;
				long			mNbCmds;
				ConsoleCommand*	mCmds[CONSOLE_MAX_COMMAND_NB];
				void*			mUserData;

				ConsoleRow		mBuffer[CONSOLE_MAX_ROW];
				char			mPrompt[256];
				char			mLastChar[2];
				PxI32			mViewBottom;
				PxI32			mNewline;
				PxI32			mCol;
				bool			mIsActive;

		// Internal methods
				void			cmdClear();
				void			advance();
				void			resetCol();
				void			process();
				void			in(PxU32 wparam);
				void			cmdHistory();
				bool			execCmd(const char* cmd, const char* param);
				void			destroy();
				bool			findBestCommand(char* best_command, const char* text, PxU32& tabIndex)	const;

	};

#endif
