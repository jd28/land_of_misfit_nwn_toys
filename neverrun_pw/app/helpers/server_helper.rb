require 'rails_rinku'

module ServerHelper
  def checkbox(predicate)
    if predicate
      content_tag(:div, '', class: 'far fa-check-square fa-lg')
    else
      content_tag(:div, '', class: 'far fa-square fa-lg')
    end
  end

  def gametype(type)
    case type
    when 0
      'Action'
    when 3
      'Role Play'
    when 4
      'Team'
    when 7
      'Social'
    when 9
      'PW Action'
    when 8
      'Alternative'
    when 1
      'Story'
    when 2
      'Story Lite'
    when 5
      'Melee'
    when 6
      'Arena'
    when 10
      'PW Story'
    when 11
      'Solo'
    when 12
      'Tech Support'
    else
      '<Unknown>'
    end
  end

  def markdown(text)
    md = Redcarpet::Markdown.new(Redcarpet::Render::HTML,
                                 autolink: true,
                                 tables: true)
    md.render(text)
  end

  def version_to_symbol(version)
    case version
    when 1
      :nwn1
    when 2
      :nwn2
    when 3
      :nwnee
    end
  end

  def version_to_s(version)
    case version
    when 1
      "NWN"
    when 2
      "NWN2"
    when 3
      "NWN:EE"
    end
  end

  def version_to_url(version)
    case version
    when 1
      "nwn1"
    when 2
      "nwn2"
    when 3
      "nwnee"
    end
  end
end
