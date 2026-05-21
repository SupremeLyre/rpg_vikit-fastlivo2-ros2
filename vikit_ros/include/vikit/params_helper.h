/*
 * ros_params_helper.h
 *
 *  Created on: Feb 22, 2013
 *      Author: cforster
 *
 * from libpointmatcher_ros
 */

#ifndef ROS_PARAMS_HELPER_H_
#define ROS_PARAMS_HELPER_H_

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <rclcpp/rclcpp.hpp>

namespace vk {

namespace detail {

inline std::weak_ptr<rclcpp::Node>& storedParamNode()
{
  static std::weak_ptr<rclcpp::Node> node;
  return node;
}

inline std::string normalizeParamName(const rclcpp::Node::SharedPtr& node, std::string name)
{
  while (!name.empty() && name.front() == '/')
    name.erase(name.begin());

  if (node)
  {
    const std::string node_name = node->get_name();
    const std::string slash_prefix = node_name + "/";
    const std::string dot_prefix = node_name + ".";
    if (name.rfind(slash_prefix, 0) == 0)
      name.erase(0, slash_prefix.size());
    else if (name.rfind(dot_prefix, 0) == 0)
      name.erase(0, dot_prefix.size());
  }

  std::replace(name.begin(), name.end(), '/', '.');
  return name;
}

template<typename T>
T readParam(const rclcpp::Node::SharedPtr& node, const std::string& name, const T& default_value, bool required)
{
  if (!node)
  {
    RCLCPP_ERROR(rclcpp::get_logger("vikit_ros"), "No rclcpp node is available for parameter: %s", name.c_str());
    return default_value;
  }

  const std::string param_name = normalizeParamName(node, name);
  if (!node->has_parameter(param_name))
  {
    try
    {
      node->declare_parameter<T>(param_name, default_value);
    }
    catch (const rclcpp::exceptions::ParameterAlreadyDeclaredException&)
    {
    }
  }

  T value = default_value;
  if (node->get_parameter(param_name, value))
  {
    RCLCPP_INFO(node->get_logger(), "Found parameter: %s", param_name.c_str());
    return value;
  }

  if (required)
    RCLCPP_ERROR(node->get_logger(), "Cannot find value for parameter: %s", param_name.c_str());
  else
    RCLCPP_WARN(node->get_logger(), "Cannot find value for parameter: %s, assigning default.", param_name.c_str());
  return default_value;
}

} // namespace detail

inline void setParamNode(const rclcpp::Node::SharedPtr& node)
{
  detail::storedParamNode() = node;
}

inline rclcpp::Node::SharedPtr getParamNode()
{
  return detail::storedParamNode().lock();
}

inline bool hasParam(const rclcpp::Node::SharedPtr& node, const std::string& name)
{
  return node && node->has_parameter(detail::normalizeParamName(node, name));
}

inline bool hasParam(const std::string& name)
{
  return hasParam(getParamNode(), name);
}

template<typename T>
T getParam(const rclcpp::Node::SharedPtr& node, const std::string& name, const T& defaultValue)
{
  return detail::readParam<T>(node, name, defaultValue, false);
}

template<typename T>
T getParam(const std::string& name, const T& defaultValue)
{
  return getParam<T>(getParamNode(), name, defaultValue);
}

template<typename T>
T getParam(const rclcpp::Node::SharedPtr& node, const std::string& name)
{
  return detail::readParam<T>(node, name, T(), true);
}

template<typename T>
T getParam(const std::string& name)
{
  return getParam<T>(getParamNode(), name);
}

inline std::vector<int> getParam(const rclcpp::Node::SharedPtr& node, const std::string& name, const std::vector<int>& defaultValue)
{
  std::vector<int64_t> default_value64(defaultValue.begin(), defaultValue.end());
  const std::vector<int64_t> value64 = getParam<std::vector<int64_t>>(node, name, default_value64);
  return std::vector<int>(value64.begin(), value64.end());
}

inline std::vector<int> getParam(const std::string& name, const std::vector<int>& defaultValue)
{
  return getParam(getParamNode(), name, defaultValue);
}

} // namespace vk

#endif // ROS_PARAMS_HELPER_H_
