/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <initializer_list>
#include "../../Core/Text/string_format.h"
#include "../../Display/2D/color.h"

namespace clan
{
	class Style;
	class StyleImpl;
	class StyleProperty;
	class ViewGeometry;
	class Canvas;
	class Font;
	class Colorf;
	class ImageSource;

	/// Style value type
	enum class StyleValueType
	{
		undefined,  /// value undefined
		keyword,    /// value is a keyword
		length,     /// value is a length
		percentage, /// value is a percentage number
		number,     /// value is a number
		string,     /// value is a text string
		url,        /// value is an url
		color,      /// value is a color
		angle,      /// value is an angle
		time,       /// value is a time
		frequency,  /// value is a frequency
		resolution  /// value is a resolution
	};

	/// Unit of a style value
	enum class StyleDimension
	{
		px,   /// device independent pixel (96 dpi)
		em,   /// relative to font-size length property
		pt,   /// point, 1/72 inch
		mm,   /// millimeter
		cm,   /// centimeter
		in,   /// inch, 1in is equal to 2.54cm
		pc,   /// picas, 1pc is equal to 12pt
		ex,   /// x-height, 1ex is equal to 0.5em
		ch,   /// used advance measure of the "0" glyph found in the font used to render it
		rem,  /// computed value of 'font-size' on the root element
		vw,   /// 1/100 viewport (initial containing box) width
		vh,   /// 1/100 viewport (initial containing box) height
		vmin, /// the smaller of vw or vh
		vmax, /// the larger of vw or vh
		deg,  /// degrees (360 in a full circle)
		grad, /// gradians/gons/grades (400 in a full circle)
		rad,  /// radians (2*PI in a full circle)
		turn, /// turns (1 in a full circle)
		s,    /// seconds
		ms,   /// milliseconds
		hz,   /// hertz
		khz,  /// kilohertz
		dpi,  /// dots per inch
		dpcm, /// dots per cm
		dppx  /// dots per px unit
	};

	/// Style value returned by style classes
	class StyleGetValue
	{
	public:
		/// Variant type
		StyleValueType type() const { return _type; }

		/// Text when the type is a text string
		const char *text() const
		{
			switch (_type)
			{
			case StyleValueType::keyword:
			case StyleValueType::url:
			case StyleValueType::string:
				return _value.text;
			default:
				return "";
			}
		}

		/// Value number
		float number() const
		{
			switch (_type)
			{
			case StyleValueType::length:
			case StyleValueType::percentage:
			case StyleValueType::number:
			case StyleValueType::angle:
			case StyleValueType::time:
			case StyleValueType::frequency:
			case StyleValueType::resolution:
				return _value.float_value.number;
			default:
				return 0.0f;
			}
		}

		/// Dimension used by value
		StyleDimension dimension() const
		{
			switch (_type)
			{
			case StyleValueType::length:
			case StyleValueType::angle:
			case StyleValueType::time:
			case StyleValueType::frequency:
			case StyleValueType::resolution:
				return _value.float_value.dimension;
			default:
				return StyleDimension::px;
			}
		}

		/// Value color
		Colorf color() const
		{
			if (_type == StyleValueType::color)
				return Colorf(_value.color[0], _value.color[1], _value.color[2], _value.color[3]);
			else
				return Colorf();
		}

		/// Check if value is undefined
		bool is_undefined() const { return _type == StyleValueType::undefined; }

		/// Check if value is a keyword
		bool is_keyword() const { return _type == StyleValueType::keyword; }
		bool is_keyword(const char *keyword) const { return is_keyword() && strcmp(_value.text, keyword) == 0; }
		bool is_keyword(const std::string &keyword) const { return is_keyword() && _value.text == keyword; }

		/// Check if value is a length
		bool is_length() const { return _type == StyleValueType::length; }

		/// Check if value is an angle
		bool is_angle() const { return _type == StyleValueType::angle; }

		/// Check if value is a time
		bool is_time() const { return _type == StyleValueType::time; }

		/// Check if value is a frequency
		bool is_frequency() const { return _type == StyleValueType::frequency; }

		/// Check if value is a resolution
		bool is_resolution() const { return _type == StyleValueType::resolution; }

		/// Check if value is a percentage
		bool is_percentage() const { return _type == StyleValueType::percentage; }

		/// Check if value is a number
		bool is_number() const { return _type == StyleValueType::number; }

		/// Check if value is a string
		bool is_string() const { return _type == StyleValueType::string; }

		/// Check if value is an url
		bool is_url() const { return _type == StyleValueType::url; }

		/// Check if value is a color
		bool is_color() const { return _type == StyleValueType::color; }

		/// Create style value from a keyword
		static StyleGetValue from_keyword(const char *keyword) { StyleGetValue v; v._type = StyleValueType::keyword; v._value.text = keyword; return v; }

		/// Create style value from a string
		static StyleGetValue from_string(const char *text) { StyleGetValue v; v._type = StyleValueType::string; v._value.text = text; return v; }

		/// Create style value from a length
		static StyleGetValue from_length(float length, StyleDimension dimension = StyleDimension::px) { StyleGetValue v; v._type = StyleValueType::length; v._value.float_value.number = length; v._value.float_value.dimension = dimension; return v; }

		/// Create style value from an angle
		static StyleGetValue from_angle(float angle, StyleDimension dimension = StyleDimension::rad) { StyleGetValue v; v._type = StyleValueType::angle; v._value.float_value.number = angle; v._value.float_value.dimension = dimension; return v; }

		/// Create style value from a time
		static StyleGetValue from_time(float t, StyleDimension dimension = StyleDimension::s) { StyleGetValue v; v._type = StyleValueType::time; v._value.float_value.number = t; v._value.float_value.dimension = dimension; return v; }

		/// Create style value from a frequency
		static StyleGetValue from_frequency(float freq, StyleDimension dimension = StyleDimension::hz) { StyleGetValue v; v._type = StyleValueType::frequency; v._value.float_value.number = freq; v._value.float_value.dimension = dimension; return v; }

		/// Create style value from a resolution
		static StyleGetValue from_resolution(float resolution, StyleDimension dimension = StyleDimension::dppx) { StyleGetValue v; v._type = StyleValueType::resolution; v._value.float_value.number = resolution; v._value.float_value.dimension = dimension; return v; }

		/// Create style value from a percentage
		static StyleGetValue from_percentage(float percentage) { StyleGetValue v; v._type = StyleValueType::percentage; v._value.float_value.number = percentage; return v; }

		/// Create style value from a number
		static StyleGetValue from_number(float number) { StyleGetValue v; v._type = StyleValueType::number; v._value.float_value.number = number; return v; }

		/// Create style value from an url
		static StyleGetValue from_url(const char *url) { StyleGetValue v; v._type = StyleValueType::url; v._value.text = url; return v; }

		/// Create style value from a color
		static StyleGetValue from_color(const Colorf &color) { StyleGetValue v; v._type = StyleValueType::color; v._value.color[0] = color.r; v._value.color[1] = color.g; v._value.color[2] = color.b; v._value.color[3] = color.a; return v; }

	private:
		StyleValueType _type = StyleValueType::undefined;

		union
		{
			const char *text;
			struct
			{
				float number;
				StyleDimension dimension;
			} float_value;
			float color[4];
		} _value;
	};

	/// Style value variable
	class StyleSetValue
	{
	public:
		/// Variant type
		StyleValueType type = StyleValueType::undefined;

		/// Text when the type is a text string
		std::string text;

		/// Value number
		float number = 0.0f;

		/// Dimension used by value
		StyleDimension dimension = StyleDimension::px;

		/// Value color
		Colorf color;

		/// Check if value is undefined
		bool is_undefined() const { return type == StyleValueType::undefined; }

		/// Check if value is a keyword
		bool is_keyword() const { return type == StyleValueType::keyword; }
		bool is_keyword(const char *keyword) const { return is_keyword() && text == keyword; }
		bool is_keyword(const std::string &keyword) const { return is_keyword() && text == keyword; }

		/// Check if value is a length
		bool is_length() const { return type == StyleValueType::length; }

		/// Check if value is an angle
		bool is_angle() const { return type == StyleValueType::angle; }

		/// Check if value is a time
		bool is_time() const { return type == StyleValueType::time; }

		/// Check if value is a frequency
		bool is_frequency() const { return type == StyleValueType::frequency; }

		/// Check if value is a resolution
		bool is_resolution() const { return type == StyleValueType::resolution; }

		/// Check if value is a percentage
		bool is_percentage() const { return type == StyleValueType::percentage; }

		/// Check if value is a number
		bool is_number() const { return type == StyleValueType::number; }

		/// Check if value is a string
		bool is_string() const { return type == StyleValueType::string; }

		/// Check if value is an url
		bool is_url() const { return type == StyleValueType::url; }

		/// Check if value is a color
		bool is_color() const { return type == StyleValueType::color; }

		/// Create style value from a keyword
		static StyleSetValue from_keyword(const std::string &keyword) { StyleSetValue v; v.type = StyleValueType::keyword; v.text = keyword; return v; }

		/// Create style value from a string
		static StyleSetValue from_string(const std::string &text) { StyleSetValue v; v.type = StyleValueType::string; v.text = text; return v; }

		/// Create style value from a length
		static StyleSetValue from_length(float length, StyleDimension dimension = StyleDimension::px) { StyleSetValue v; v.type = StyleValueType::length; v.number = length; v.dimension = dimension; return v; }

		/// Create style value from an angle
		static StyleSetValue from_angle(float angle, StyleDimension dimension = StyleDimension::rad) { StyleSetValue v; v.type = StyleValueType::angle; v.number = angle; v.dimension = dimension; return v; }

		/// Create style value from a time
		static StyleSetValue from_time(float t, StyleDimension dimension = StyleDimension::s) { StyleSetValue v; v.type = StyleValueType::time; v.number = t; v.dimension = dimension; return v; }

		/// Create style value from a frequency
		static StyleSetValue from_frequency(float freq, StyleDimension dimension = StyleDimension::hz) { StyleSetValue v; v.type = StyleValueType::frequency; v.number = freq; v.dimension = dimension; return v; }

		/// Create style value from a resolution
		static StyleSetValue from_resolution(float resolution, StyleDimension dimension = StyleDimension::dppx) { StyleSetValue v; v.type = StyleValueType::resolution; v.number = resolution; v.dimension = dimension; return v; }

		/// Create style value from a percentage
		static StyleSetValue from_percentage(float percentage) { StyleSetValue v; v.type = StyleValueType::percentage; v.number = percentage; return v; }

		/// Create style value from a number
		static StyleSetValue from_number(float number) { StyleSetValue v; v.type = StyleValueType::number; v.number = number; return v; }

		/// Create style value from an url
		static StyleSetValue from_url(const std::string &url) { StyleSetValue v; v.type = StyleValueType::url; v.text = url; return v; }

		/// Create style value from a color
		static StyleSetValue from_color(const Colorf &color) { StyleSetValue v; v.type = StyleValueType::color; v.color = color; return v; }
	};

	/// Gradient stop in a style gradient
	class StyleGradientStop
	{
	public:
		StyleGradientStop() { }
		StyleGradientStop(const StyleSetValue &color, const StyleSetValue &position) : color(color), position(position) { }

		StyleSetValue color;
		StyleSetValue position;
	};

	/// Linear or radial gradient
	class StyleGradient
	{
	public:
		StyleSetValue type;
		StyleSetValue linear_angle;
		StyleSetValue radial_shape;
		StyleSetValue radial_size_x;
		StyleSetValue radial_size_y;
		StyleSetValue radial_position_x;
		StyleSetValue radial_position_y;
		std::vector<StyleGradientStop> stops;
	};

	/// Style image values
	class StyleImage
	{
	public:
		StyleImage() { }
		StyleImage(const StyleSetValue &image) : image(image) { }
		StyleImage(const StyleGradient &gradient) : gradient(gradient) { }

		StyleSetValue image;
		StyleGradient gradient;
	};
	
	/// Style value resolver
	class StyleCascade
	{
	public:
		StyleCascade() { }
		StyleCascade(std::vector<Style *> cascade, StyleCascade *parent = nullptr) : cascade(std::move(cascade)), parent(parent) { }

		/// Property sets to be examined
		std::vector<Style *> cascade;

		/// Parent cascade used for inheritance
		StyleCascade *parent = nullptr;
		
		/// Find the first declared value in the cascade for the specified property
		StyleGetValue cascade_value(const std::string &property_name) const;

		/// Resolve any inheritance or initial values for the cascade value
		StyleGetValue specified_value(const std::string &property_name) const;

		/// Find the computed value for the specified value
		///
		/// The computed value is a simplified value for the property. Lengths are resolved to device independent pixels and so on.
		StyleGetValue computed_value(const std::string &property_name) const;
		
		/// Convert length into px (device independent pixel) units
		StyleGetValue compute_length(const StyleGetValue &length) const;

		/// Convert angle into radians
		StyleGetValue compute_angle(const StyleGetValue &angle) const;

		/// Convert time to seconds
		StyleGetValue compute_time(const StyleGetValue &time) const;

		/// Convert frequency to Hz
		StyleGetValue compute_frequency(const StyleGetValue &frequency) const;

		/// Convert resolution to dots per px unit (pixel ratio scale)
		StyleGetValue compute_resolution(const StyleGetValue &resolution) const;
		
		/// Value array size for the property
		int array_size(const std::string &property_name) const;
		
		/// Render styled background
		void render_background(Canvas &canvas, const ViewGeometry &geometry) const;

		/// Render styled border
		void render_border(Canvas &canvas, const ViewGeometry &geometry) const;
		
		/// Font used by this style cascade
		Font get_font(Canvas &canvas) const;
	};

	/// Style property set
	class Style
	{
	public:
		Style();
		Style(const Style &) = delete;
		~Style();
		Style &operator=(const Style &) = delete;

		/// Parse and apply CSS properties to property set
		///
		/// The properties string uses the same syntax as when styles are specified using the 'style' attribute in HTML.
		/// This function does not clear the current properties already set and can be called multiple times with
		/// individual sets of properties.
		///
		/// Additional arguments can be passed in and will be inserted with the same syntax as when using the
		/// string_format function.
		void set(const std::string &properties);

		template <class Arg1, typename... Values>
		void set(const std::string &properties, Arg1 arg1, Values... values)
		{
			set(string_format(properties, arg1, values...));
		}

		/// Retrieve the declared value for a property
		StyleGetValue declared_value(const std::string &property_name) const;

		/// Static helper that generates a "rgba(%1,%2,%3,%4)" string for the given color.
		static std::string to_rgba(const Colorf &c)
		{
			return string_format(
				"rgba(%1,%2,%3,%4)",
				clamp((int)std::round(c.r * 255), 0, 255),
				clamp((int)std::round(c.g * 255), 0, 255),
				clamp((int)std::round(c.b * 255), 0, 255),
				c.a);
		}

	private:
		std::unique_ptr<StyleImpl> impl;
	};
}
