/**
 *  @file    ei_geometrymanager_placeur.c
 *  @brief   Implements the "placer" geometry manager for widgets.
 *
 *  \author
 *  Created by BOON, MOHD NOOR and NAIR on 21.05.24.
 *
 */

#include "ei_geometrymanager.h"
#include "ei_implementation.h"

/**
 * \brief	Calculates and sets the new location and size of a widget according to the placer parameters.
 * 		<ul>
 * 			<li> Retrieves the placer parameters from the widget. </li>
 * 			<li> Allocates memory for the new location rectangle. </li>
 * 			<li> If the widget has a parent, obtains the parent's width and height. </li>
 * 			<li> Calculates the new coordinates based on the parent's dimensions and the placer parameters. </li>
 * 			<li> Adjusts the widget's dimensions based on relative width and height, if specified. </li>
 * 			<li> Sets the new top-left corner of the widget according to the specified anchor. </li>
 * 			<li> Finalizes the geometry of the widget by setting its new location and size. </li>
 * 			<li> Frees the allocated memory for the new location rectangle. </li>
 * 		</ul>
 * @param	widget		The widget whose location and size are to be set.
 */
void placeur_runfunc(ei_widget_t widget);

/**
 * \brief	Placeholder function for releasing resources of the placer geometry manager.
 *
 * This function currently has no implementation since there are no resources to release for the placer.
 *
 * @param	widget		The widget whose geometry manager is to be released.
 */
void placeur_releasefunc(ei_widget_t widget);

/**
 * \brief	Creates and initializes a new instance of the placer geometry manager.
 * 		<ul>
 * 			<li> Allocates memory for a new ei_geometrymanager_t instance. </li>
 * 			<li> Sets the name of the geometry manager to "placeur". </li>
 * 			<li> Assigns the run function and release function pointers. </li>
 * 			<li> Returns the initialized geometry manager instance. </li>
 * 		</ul>
 *
 * @return			A pointer to the newly created ei_geometrymanager_t instance.
 */
ei_geometrymanager_t* create_placeur_mng();