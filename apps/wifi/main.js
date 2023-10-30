/*©mit**************************************************************************
*                                                                              *
* This file is part of FRIEND UNIFYING PLATFORM.                               *
* Copyright (c) Friend Software Labs AS. All rights reserved.                  *
*                                                                              *
* Licensed under the Source EULA. Please refer to the copy of the MIT License, *
* found in the file LICENSE.                                                   *
*                                                                              *
*****************************************************************************©*/

Application.run = function( msg )
{
	let v = new View( {
		title: i18n( 'i18n_power' ),
		width: 640,
		height: 640,
		assets: [
			'Progdir:assets/main.html'
		]
	} );
	v.onClose = function()
	{
		Application.quit();
	}
	this.mainView = v;
}

