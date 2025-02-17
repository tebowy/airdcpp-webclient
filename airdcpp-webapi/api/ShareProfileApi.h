/*
* Copyright (C) 2011-2024 AirDC++ Project
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef DCPLUSPLUS_DCPP_SHAREPROFILE_API_H
#define DCPLUSPLUS_DCPP_SHAREPROFILE_API_H

#include <api/base/SubscribableApiModule.h>

#include <airdcpp/core/header/typedefs.h>
#include <airdcpp/share/profiles/ShareProfileManagerListener.h>

namespace dcpp {
	class ShareProfileManager;
}

namespace webserver {
	class ShareProfileApi : public SubscribableApiModule, private ShareProfileManagerListener {
	public:
		ShareProfileApi(Session* aSession);
		~ShareProfileApi();
	private:
		static json serializeShareProfile(const ShareProfilePtr& aProfile) noexcept;

		api_return handleGetProfiles(ApiRequest& aRequest);
		api_return handleGetProfile(ApiRequest& aRequest);

		api_return handleAddProfile(ApiRequest& aRequest);
		api_return handleUpdateProfile(ApiRequest& aRequest);
		api_return handleRemoveProfile(ApiRequest& aRequest);

		api_return handleGetDefaultProfile(ApiRequest& aRequest);
		api_return handleSetDefaultProfile(ApiRequest& aRequest);

		void updateProfileProperties(ShareProfilePtr& aProfile, const json& j);

		void on(ShareProfileManagerListener::ProfileAdded, ProfileToken aProfile) noexcept override;
		void on(ShareProfileManagerListener::ProfileUpdated, ProfileToken aProfile, bool aIsMajorChange) noexcept override;
		void on(ShareProfileManagerListener::ProfileRemoved, ProfileToken aProfile) noexcept override;

		ShareProfilePtr parseProfileToken(ApiRequest& aRequest, bool aAllowHidden);

		ShareProfileManager& mgr;
	};
}

#endif